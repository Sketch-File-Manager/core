#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include <session_parser.h>
#include <file_handler.h>
#include <codes.h>
#include <functions.h>

#define SESSION_FOLDER    "/.local/share/sketch/sessions/"
#define SESSION_LOCATION  "/.local/share/sketch/"

int delete_file(const char *name) {
    char *absolute_path = get_absolute_path(name, SESSION_FOLDER);
    // remove file with name.
    remove(absolute_path);

    free(absolute_path);
    return 0;
}

int create_file(const char *name) {
    char *absolute = get_absolute_path(name, SESSION_FOLDER);
    // make a new file with name.
    int new_fd = open(absolute, O_CREAT, 0700);

    close(new_fd);
    free(absolute);
    return 0;
}

static inline char *double_array_to_string(const char **d_array, size_t size) {

    if (size == 1) {
        char *tmp = calloc(strlen(d_array[0]) + 1, sizeof(char));
        strcpy(tmp, d_array[0]);
        return tmp;
    }

    // allocate the space of the new string.
    size_t total_size = 1 + size;
    char *string_form = calloc(1 + size, sizeof(char));
    // save each element on the as sequence of strings.
    for (int element = 0; element < size; element++) {
        // Increase the space dynamically.
        total_size += strlen(d_array[element]);
        string_form = realloc(string_form, sizeof(char) * total_size);
        strcat(string_form, d_array[element]);
        strcat(string_form, "\n");
    }

    return string_form;
}

int delete_last_line(const char *name) {
    // Get the absolute path.
    char *absolute_path = get_absolute_path(name, SESSION_FOLDER);
    char *session_file = NULL;

    // read the file.
    if (read_file(absolute_path, &session_file) == -1) return -1;

    // Allocate space for one element.
    size_t session_lines_s = 1;
    char **session_lines = calloc(1, sizeof(char *));

    // Get the first line.
    char *current_line = strtok(session_file, "\n");

    // If no line exist.
    if (current_line == NULL) return  -1;

    // Fill the array with the lines in the session file.
    for (size_t s = 0; current_line != NULL; s++) {
        // Allocate space for the current line.
        session_lines[s] = calloc(strlen(current_line) + 1, sizeof(char));
        strcpy(session_lines[s], current_line);
        // Increase the line number in the array, because we have more lines.
        ++session_lines_s;
        // Re allocate space to fit the new line.
        session_lines = realloc(session_lines, sizeof(char *) * session_lines_s);
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
    if (write_file(absolute_path, new_session_file, strlen(new_session_file)) == -1) {
        free(absolute_path);
        free(new_session_file);
        return -1;
    }

    free(absolute_path);
    free(new_session_file);

    return 0;
}

static int append(const char *name, const char *content, int is_start) {
    char *relative_path = get_absolute_path(name, SESSION_FOLDER);
    char *session_file = NULL;

    if (read_file(relative_path, &session_file) == -1) return -1;

    size_t new_file_s = strlen(content) + strlen(session_file);
    char *new_file = calloc(new_file_s + 2, sizeof(char));

    if (is_start == TRUE) {
        strcpy(new_file, content);
        strcat(new_file, "\n");
        strcat(new_file, session_file);
    }
    else {
        strcpy(new_file, session_file);
        strcat(new_file, content);
        strcat(new_file, "\n");
    }

    if (write_file(relative_path, new_file, new_file_s + 1) == -1) return -1;

    free(relative_path);

    return 0;
}

int append_to_end(const char *name, const char *content) {
    return append(name, content, FALSE);
}

int append_to_start(const char *name, const char *content) {
    return append(name, content, TRUE);
}

int read_session(const char *name, char ***result, size_t *size) {
    char *absolute_path = get_absolute_path(name, SESSION_FOLDER);
    char *session_file = NULL;

    if (read_file(absolute_path, &session_file) == -1) return -1;

    char **lines;
    char *current_line = strtok(session_file, "\n");

    if (current_line == NULL) {
        lines = calloc(1, sizeof(char *));
        lines[0] = calloc(1, sizeof(char));
        strcpy(lines[0], "");
        *size = 0;
        *result = lines;
        return SUCCESS;
    }

    size_t session_file_lines_s = 1;
    lines = calloc(1, sizeof(char *));

    lines[0] = calloc(strlen(current_line) + 1, sizeof(char));
    strcpy(lines[0], current_line);
    for (size_t line = 1; ; line++) {
        current_line = strtok(NULL, "\n");
        if (current_line == NULL) break;

        // Increase the size of the array.
        ++session_file_lines_s;
        lines = realloc(lines, sizeof(char *) * session_file_lines_s);
        // Allocate space for the new line.
        lines[line] = calloc(strlen(current_line) + 1, sizeof(char));
        strcpy(lines[line], current_line);
    }
    *size = session_file_lines_s;
    *result = lines;

    free(session_file);
    free(absolute_path);
    return 0;
}

int session_exists(const char* name) {
    char *absolute_path = get_absolute_path(name, SESSION_FOLDER);
    int file_fd = open(absolute_path, O_RDONLY);

    if (file_fd == -1) return FALSE;

    close(file_fd);
    return TRUE;
}

int list_sessions(char ***result, size_t *size) {
    char **files = NULL;
    size_t files_s = 0;

    char *path = get_absolute_path("sessions/", SESSION_LOCATION);
    list_files(path, &files, &files_s);

    size_t session_files_s = 1;
    char **session_files = calloc(1, sizeof(char *));

    for (int file = 0; file < files_s; file++) {
        if (endsWith(files[file], ".session") == TRUE) {
            session_files[session_files_s - 1] = calloc(strlen(files[file]) + 1, sizeof(char));
            strcpy(session_files[session_files_s - 1], files[file]);
            ++session_files_s;
            session_files = realloc(session_files, sizeof(char *) * session_files_s);
        }
        free(files[file]);
    }
    free(files);
    --session_files_s;

    *size = session_files_s;
    *result = session_files;

    return SUCCESS;
}