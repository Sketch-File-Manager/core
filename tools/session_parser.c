#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include <session_parser.h>
#include <file_handler.h>
#include <codes.h>

#define SESSION_FOLDER      "sessions/"
#define SESSION_LOCATION    "/.local/share/sketch/"

static inline char *get_absolute_path(char *session_file_name) {
    char *username = getlogin();
    size_t absolute_path_s = strlen(username) + strlen(SESSION_FOLDER) + strlen("/home/") + strlen(SESSION_LOCATION) + strlen(session_file_name);
    char *absolute_path = calloc(absolute_path_s + 2, sizeof(char));
    strcpy(absolute_path, "/home/");
    strcat(absolute_path, username);
    strcat(absolute_path, SESSION_LOCATION);
    strcat(absolute_path, SESSION_FOLDER);
    strcat(absolute_path, session_file_name);

    return absolute_path;
}

int delete_file(char *name) {
    char *absolute_path = get_absolute_path(name);
    // remove file with name.
    remove(absolute_path);

    free(absolute_path);
    return 0;
}

int create_file(char *name) {
    char *absolute = get_absolute_path(name);
    // make a new file with name.
    int new_fd = open(absolute, O_CREAT, 0700);

    close(new_fd);
    free(absolute);
    return 0;
}

static inline size_t get_session_lines(char *session_file) {
    char tmp[strlen(session_file) + 1];
    strcpy(tmp, session_file);

    char *current_line = strtok(tmp, "\n");
    size_t size = 0;

    while (current_line) {
        ++size;
        current_line = strtok(NULL, "\n");
    }

    return size;
}

static inline char *double_array_to_string(char **d_array, size_t size) {
    size_t total_size = 0;

    if (size == 1) {
        char *tmp = calloc(strlen(d_array[0]) + 1, sizeof(char));
        strcpy(tmp, d_array[0]);
        return tmp;
    }

    // calculate the total size of the new string.
    for (int element = 0; element < size; element++) total_size += strlen(d_array[element]);

    // allocate the space of the new string.
    char *string_form = calloc(total_size + size + 1, sizeof(char));
    // save each element on the as sequence of strings.
    for (int element = 0; element < size; element++) {
        strcat(string_form, d_array[element]);
        strcat(string_form, "\n");
    }

    return string_form;
}

int delete_last_line(char *name) {
    char *relative_path = calloc(strlen(SESSION_FOLDER) + strlen(name) + 1, sizeof(char));
    char *session_file = NULL;

    strcat(relative_path, SESSION_FOLDER);
    strcat(relative_path, name);

    if (read_file(relative_path, &session_file) == -1) return -1;

    size_t session_lines_s = get_session_lines(session_file);
    char **session_lines = calloc(session_lines_s, sizeof(char *));

    if (session_lines_s == 0) return -1;

    char tmp[strlen(session_file) + 1];
    strcpy(tmp, session_file);

    printf("%d", (int) session_lines_s);
    session_lines[0] = strtok(tmp, "\n");
    for (size_t s = 1; s < session_lines_s - 1; s++) {
        session_lines[s] = strtok(NULL, "\n");
    }
    char *new_session_file = double_array_to_string(session_lines, session_lines_s - 1);

    if (write_file(relative_path, new_session_file, strlen(new_session_file)) == -1) return -1;

    free(session_file);
    free(relative_path);
    free(session_lines);
    free(new_session_file);
    return 0;
}

static int append(char *name, char *content, int is_start) {
    char *relative_path = calloc(strlen(SESSION_FOLDER) + strlen(name) + 1, sizeof(char));
    char *session_file = NULL;

    strcat(relative_path, SESSION_FOLDER);
    strcat(relative_path, name);
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

int append_to_end(char *name, char *content) {
    return append(name, content, FALSE);
}

int append_to_start(char *name, char *content) {
    return append(name, content, TRUE);
}

int read_session(char *name, char ***result, size_t *size) {
    char *relative_path = calloc(strlen(SESSION_FOLDER) + strlen(name) + 1, sizeof(char));
    char *session_file = NULL;

    strcat(relative_path, SESSION_FOLDER);
    strcat(relative_path, name);

    if (read_file(relative_path, &session_file) == -1) return -1;

    size_t session_file_lines_s = get_session_lines(session_file);
    char **lines = calloc(session_file_lines_s, sizeof(char *));

    char *current_line = strtok(session_file, "\n");
    lines[0] = calloc(strlen(current_line) + 1, sizeof(char));
    strcpy(lines[0], current_line);
    for (size_t line = 1; line < session_file_lines_s; line++) {
        current_line = strtok(NULL, "\n");
        lines[line] = calloc(strlen(current_line) + 1, sizeof(char));
        strcpy(lines[line], current_line);
    }
    *size = session_file_lines_s;
    *result = lines;

    free(session_file);
    free(relative_path);
    return 0;
}

int session_exists(char* name) {
    char *absolute_path = get_absolute_path(name);
    int file_fd = open(absolute_path, O_RDONLY);

    if (file_fd == -1) return 0;

    close(file_fd);
    return 1;
}