#include <string.h>
#include <stdio.h>
#include <codes.h>
#include <sys/stat.h>
#include <functions.h>
#include <stdlib.h>

#include "executor.h"
#include "../commands/commands.h"


/**
 * Splits a string to an array of strings by a specific delimiter. It will skip the split by delimiter if the previous character is the same as the prev_delim_except.
 * @param str The string that will be split.
 * @param delimiter The delimiter.
 * @param prev_delim_except The exception character that is located before the delimiter.
 * @param n The size of the array that will be returned.
 * @return A string array that contains the split form of the str.
 */
static char** split_with_exception(char* str, char delimiter, char prev_delim_except, size_t* n) {
    char** ret = (char**) calloc(1, sizeof (char*));
    int a = 0;

    char* token = calloc(strlen(str) + 1, sizeof(char));
    int k = 0;
    for (int i = 0; str[i]; i++) {
        if(str[i] != delimiter || (i > 0 && str[i - 1] != prev_delim_except)) {
            token[k] = str[i];
            k++;
            continue;
        }

        ret[a] = calloc(strlen(token) + 1, sizeof(char));
        strcpy(ret[a], token);
        ret = realloc(ret, (a + 1) + 1);
        a++;

        token = "";
        k = 0;
    }

    *n = a + 1;
    free(token);
    return ret;
}

/**
 * Translate a string of permissions to bitmask.
 * @param p The permission string, e.x. "0700"
 * @return The permission.
 */
static __mode_t parse_permission(char* p) {
    return (__mode_t) p;
}

int execute(char* command) {
    size_t n;
    char** split = split_with_exception(command, ' ', '\\', &n);

    if(strcmp(split[0], "mkdir") == 0) {
        char* dst_folder = fix_path(split[1], TRUE);
        char* name = split[2];
        __mode_t permissions = parse_permission(split[3]);

        return command_mkdir(dst_folder, name, permissions);
    }
    else if(strcmp(split[0], "mkfile") == 0) {
        char* dst_folder = fix_path(split[1], TRUE);
        char* file = split[2];
        __mode_t permissions = parse_permission(split[3]);

        return command_mkfile(dst_folder, file, permissions);
    }
    else if(strcmp(split[0], "copy") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* dst_folder = fix_path(split[2], TRUE);

        return command_copy(src, dst_folder);
    }
    else if(strcmp(split[0], "move") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* dst_folder = fix_path(split[2], TRUE);

        return command_move(src, dst_folder);
    }
    else if(strcmp(split[0], "rename") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* new_name = split[2];

        return command_rename(src, new_name);
    }
    else if(strcmp(split[0], "edit") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* content = split[2];
        char* flag = split[3];

        return command_edit(src, content, flag);
    }
    else if(strcmp(split[0], "permissions") == 0) {
        char* src = fix_path(split[1], TRUE);
        __mode_t permissions = parse_permission(split[2]);
        unsigned int recursive = (unsigned char) split[3][0];

        return command_permissions(src, permissions, recursive);
    }

    for (int i = 0; i < n; ++i)
        free(split[i]);
    free(split);

    return UNKNOWN_COMMAND;
}