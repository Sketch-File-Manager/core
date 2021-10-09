/* session_parser.c */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <session_parser.h>
#include <file_handler.h>
#include <include/codes.h>
#include <include/functions.h>
#include <mem.h>


int delete_file(const char *name) {
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *absolute_path = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);

    // remove file with name.
    int result = remove(absolute_path);
    free(absolute_path);

    if (result == 0)
        return SUCCESS;

    return errno;
}

int create_file(const char *name) {
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *absolute = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);

    // make a new file with name.
    int new_fd = open(absolute, O_CREAT, 0700);
    free(absolute);

    if (new_fd == -1)
        return errno;
    if (close(new_fd) == -1)
        return errno;

    return SUCCESS;
}

static inline char *double_array_to_string(const char **d_array, size_t size) {

    if (size == 1) {
        char *tmp;
        ALLOCATE_MEM(tmp, strlen(d_array[0]) + 1, sizeof(char));
        strcpy(tmp, d_array[0]);
        return tmp;
    }

    // allocate the space of the new string.
    size_t total_size = 1 + size;
    char *string_form;
    ALLOCATE_MEM(string_form, size + 1, sizeof(char));
    // save each element on the as sequence of strings.
    for (int element = 0; element < size; element++) {
        // Increase the space dynamically.
        total_size += strlen(d_array[element]);
        REALLOCATE_MEM(string_form, sizeof(char) * total_size);
        strcat(string_form, d_array[element]);
        strcat(string_form, "\n");
    }

    return string_form;
}

int delete_last_line(const char *name) {
    // Get the absolute path.
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *absolute = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);
    char *session_file = NULL;

    // read the file.
    int read_result = read_file(absolute, &session_file);
    if (read_result != SUCCESS) return read_result;

    // Allocate space for one element.
    size_t session_lines_s = 1;
    char **session_lines;
    ALLOCATE_MEM(session_lines, 1, sizeof(char *));

    // Get the first line.
    char *current_line = strtok(session_file, "\n");

    // If no line exist.
    if (current_line == NULL) return CANNOT_UNDO_ZERO_COMMANDS_FOUND;

    // Fill the array with the lines in the session file.
    for (size_t s = 0; current_line != NULL; s++) {
        // Allocate space for the current line.
        ALLOCATE_MEM(session_lines[s], strlen(current_line) + 1, sizeof(char));
        strcpy(session_lines[s], current_line);
        // Increase the line number in the array, because we have more lines.
        ++session_lines_s;
        // Re allocate space to fit the new line.
        REALLOCATE_MEM(session_lines, sizeof(char *) * session_lines_s);
        // Jump to the next line.
        current_line = strtok(NULL, "\n");
    }

    // Get the changes in string form. We just get all the lines except the last one, because we want to delete the last line.
    char *new_session_file = double_array_to_string((const char **) session_lines, session_lines_s - 2);

    // Free
    for (int fr = 0; fr < session_lines_s - 1; fr++) free(session_lines[fr]);
    free(session_file);
    free(session_lines);

    // Write the changes.
    int write_result = write_file(absolute, new_session_file, strlen(new_session_file));
    free(absolute);
    free(new_session_file);

    if (write_result != SUCCESS)
        return write_result;

    return SUCCESS;
}

static int append_to(const char *name, const char *content, int is_start) {
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *relative = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);

    char *session_file = NULL;

    int read_result = read_file(relative, &session_file);
    if (read_result != SUCCESS) return read_result;

    size_t new_file_s = strlen(content) + strlen(session_file);
    char *new_file;
    ALLOCATE_MEM(new_file, new_file_s + 2, sizeof(char));

    if (is_start == TRUE) {
        strcpy(new_file, content);
        strcat(new_file, "\n");
        strcat(new_file, session_file);
    } else {
        strcpy(new_file, session_file);
        strcat(new_file, content);
        strcat(new_file, "\n");
    }

    int write_result = write_file(relative, new_file, new_file_s + 1);
    free(relative);

    if (write_result != SUCCESS)
        return write_result;

    return SUCCESS;
}

int append_to_end(const char *name, const char *content) {
    return append_to(name, content, FALSE);
}

int append_to_start(const char *name, const char *content) {
    return append_to(name, content, TRUE);
}

int read_session(const char *name, char ***result, size_t *size) {
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *absolute = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);

    char *session_file = NULL;

    int read_result = read_file(absolute, &session_file);
    if (read_result != SUCCESS) return read_result;

    char **lines;
    char *current_line = strtok(session_file, "\n");

    if (current_line == NULL) {
        ALLOCATE_MEM(lines, 1, sizeof(char *));
        ALLOCATE_MEM(lines[0], 1, sizeof(char));
        strcpy(lines[0], "");
        *size = 0;
        *result = lines;
        return SUCCESS;
    }

    size_t session_file_lines_s = 1;
    ALLOCATE_MEM(lines, 1, sizeof(char *));
    ALLOCATE_MEM(lines[0], strlen(current_line) + 1, sizeof(char));
    strcpy(lines[0], current_line);

    for (size_t line = 1;; line++) {
        current_line = strtok(NULL, "\n");
        if (current_line == NULL) break;

        // Increase the size of the array.
        ++session_file_lines_s;
        lines = realloc(lines, sizeof(char *) * session_file_lines_s);
        // Allocate space for the new line.
        ALLOCATE_MEM(lines[line], strlen(current_line) + 1, sizeof(char));
        strcpy(lines[line], current_line);
    }
    *size = session_file_lines_s;
    *result = lines;

    free(session_file);
    free(absolute);
    return SUCCESS;
}

int session_exists(const char *name) {
    char *with_home = fix_path(SESSION_FOLDER_LOCATION, TRUE);
    char *absolute = str_add(with_home, SESSION_FOLDER_LOCATION, name, NULL);
    free(with_home);

    int file_fd = open(absolute, O_RDONLY);

    if (file_fd == -1) return FALSE;

    close(file_fd);
    return TRUE;
}

int list_sessions(char ***result, size_t *size) {
    char **files = NULL;
    size_t files_s = 0;

    char *path = fix_path(SESSION_FOLDER_LOCATION, TRUE);

    int list_result = list_files_names(path, &files, &files_s);

    if (list_result == SUCCESS) {
        size_t session_files_s = 1;
        char **session_files;
        ALLOCATE_MEM(session_files, 1, sizeof(char *));

        for (int file = 0; file < files_s; file++) {
            if (endsWith(files[file], ".session") == TRUE) {
                ALLOCATE_MEM(session_files[session_files_s - 1], strlen(files[file]) + 1, sizeof(char));
                strcpy(session_files[session_files_s - 1], files[file]);
                ++session_files_s;
                REALLOCATE_MEM(session_files, sizeof(char *) * session_files_s);
            }
            free(files[file]);
        }
        free(files);
        --session_files_s;

        *size = session_files_s;
        *result = session_files;
    } else return list_result;

    free(path);
    return SUCCESS;
}