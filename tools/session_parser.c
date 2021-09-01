#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include <session_parser.h>
#include <file_handler.h>
#include <codes.h>

#define SESSION_FOLDER "sessions/"


static char *get_absolute_path(char *session_file_name) {
    char *username = getlogin();
    size_t absolute_path_s = strlen(username) + strlen(SESSION_FOLDER) + strlen("/home/") + strlen(session_file_name);
    char *absolute_path = calloc(absolute_path_s + 1, sizeof(char));
    strcpy(absolute_path, "/home/");
    strcat(absolute_path, username);
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
    int new_fd = open(name, O_CREAT);

    close(new_fd);
    return 0;
}

static size_t get_session_lines(char *session_file) {
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

static char *double_array_to_string(char **d_array, size_t size, size_t* total_len) {
    size_t total_size = 0;

    for (int s_size = 0; s_size < size; s_size++) strlen(d_array[s_size]);

    char *string = calloc(total_size + size + 2, sizeof(char));

    for (int set = 0; set < size; set++) {
        strcat(string, d_array[set]);
        strcat(string, "\n");
    }

    *total_len = total_size;
    return string;
}

int delete_last_line(char *name) {
    /*char *absolute_path = get_absolute_path(name);

    char *session_file = NULL;

    if (read_file(absolute_path, &session_file) == -1) return -1;

    size_t session_lines_s = get_session_lines(session_file);
    char **session_lines = calloc(session_lines_s, sizeof(char *));

    if (session_lines_s == 0) return -1;

    char tmp[strlen(session_file) + 1];
    strcpy(tmp, session_file);

    session_lines[0] = strtok(tmp, "\n");
    for (size_t s = 1; s < session_lines_s - 1; s++) {
        session_lines[s] = strtok(NULL, "\n");
    }

    size_t total_new_len = 0;
    char *new_session_file = double_array_to_string(session_lines, session_lines_s - 1, &total_new_len);

    printf("%s\n", new_session_file);
    //if (write_file(absolute_path, new_session_file, total_new_len) == -1) return -1;

    free(session_file);
    free(session_lines);
    free(new_session_file);*/
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
        strcpy(new_file, session_file);
    }
    else {
        strcpy(new_file, session_file);
        strcat(new_file, content);
        strcat(new_file, "\n");
    }

    printf("%s\n", new_file);
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
    return 0;
}

int session_exists(char* name) {
    return 1;
}