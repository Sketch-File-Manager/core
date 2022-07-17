#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include "session_utils.h"

static void append(FILE *session, const char *data) {
    fprintf(session, "%s", data);
}

int session_mkdir(FILE *session, const char *dst, const char *name, const char *perms) {
    append(session, "[start]");
    append(session, "mkdir");
    append(session, dst);
    append(session, name);
    append(session, perms);
    append(session, "[end]");
    return 0;
}

int session_mkfile(FILE *session, const char *dst, const char *name, const char *perms) {
    append(session, "[start]");
    append(session, "mkfile");
    append(session, dst);
    append(session, name);
    append(session, perms);
    append(session, "[end]");
    return 0;
}

int session_copy(FILE *session, const char *dst, const char *src) {
    append(session, "[start]");
    append(session, "copy");
    append(session, dst);
    append(session, src);
    append(session, "[end]");
    return 0;
}

int session_move(FILE *session, const char *dst, const char *src) {
    append(session, "[start]");
    append(session, "move");
    append(session, dst);
    append(session, src);
    append(session, "[end]");
    return 0;
}

int session_rename(FILE *session, const char *new_name, const char *src) {
    append(session, "[start]");
    append(session, "rename");
    append(session, new_name);
    append(session, src);
    append(session, "[end]");
    return 0;
}

int session_permissions(FILE *session, const char *new_perms, const char *src) {
    append(session, "[start]");
    append(session, "permissions");
    append(session, new_perms);
    append(session, src);
    append(session, "0"); // depth
    append(session, "[end]");
    return 0;
}

int session_permissions_recursive(FILE *session, const char *new_perms, const char *src, unsigned int depth) {
    append(session, "[start]");
    append(session, "permissions");
    append(session, new_perms);
    append(session, src);
    fprintf(session, "%d", depth);
    append(session, "[end]");
    return 0;
}

int session_list(const char *session, const char *src) {
    FILE *session_file = fopen(session, "r");
    if (!session_file) return 1;

    // TODO

    fclose(session_file);
    return 0;
}

int session_delete(FILE *session, const char *src) {
    append(session, "[start]");
    append(session, "delete");
    append(session, src);
    append(session, "[end]");
    return 0;
}

char *read_last_line(FILE *file) {
    //next to last char, last is EOF
    fseek(file, -1, SEEK_END);
    int c = fgetc(file);

    //define macro EOL
    while (c == '\n') {
        fseek(file, -2, SEEK_CUR);
        c = fgetc(file);
    }

    int len = 0;
    while (c != '\n') {
        fseek(file, -2, SEEK_CUR);
        len++;
        c = fgetc(file);
    }

    fseek(file, 1, SEEK_CUR);

    char *line = calloc(len, sizeof(char));
    if (fgets(line, len, file) != NULL)
        return line;
    else
        return NULL;
}

void delete_last_line(FILE *file) {
    // TODO
}

int session_undo(FILE *session, char *session_filename) {
    char *last_line = read_last_line(session);
    while (last_line != NULL && strcmp(last_line, "[end]") != 0) {
        delete_last_line(session);
        last_line = read_last_line(session);
    }

    if(last_line == NULL)
        remove(session_filename);

    return 0;
}

int session_execute(const char *session, int keep) {
    FILE *session_file = fopen(session, "r");
    if (!session_file) return 1;

    // TODO

    fclose(session_file);
    return 0;
}
