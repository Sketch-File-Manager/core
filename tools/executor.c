/* executor.c */
#include <string.h>
#include <stdlib.h>

#include <execute/executor.h>
#include <commands.h>
#include "../constants/include/codes.h"
#include "../general functions/include/functions.h"

/**
 * Translate a string of permissions to bitmask.
 * @param p The permission string, e.x. "0700"
 * @return The permission.
 */
static inline __mode_t parse_permission(char *p) {
    return (__mode_t) p;
}

int execute(char *command) {
    size_t n;
    char **array = split_except(command, ' ', '\\', &n);


    int result = UNKNOWN_COMMAND;
    if (strcmp(array[0], "mkdir") == 0) {
        char *dst_folder = fix_path(array[1], TRUE);
        char *name = array[2];
        __mode_t permissions = parse_permission(array[3]);

        result = command_mkdir(dst_folder, name, permissions);
        free(dst_folder);
    } else if (strcmp(array[0], "mkfile") == 0) {
        char *dst_folder = fix_path(array[1], TRUE);
        char *file = array[2];
        __mode_t permissions = parse_permission(array[3]);

        result = command_mkfile(dst_folder, file, permissions);
        free(dst_folder);
    } else if (strcmp(array[0], "copy") == 0) {
        char *src = fix_path(array[1], TRUE);
        char *dst_folder = fix_path(array[2], TRUE);

        result = command_copy(src, dst_folder);
        free(src);
        free(dst_folder);
    } else if (strcmp(array[0], "move") == 0) {
        char *src = fix_path(array[1], TRUE);
        char *dst_folder = fix_path(array[2], TRUE);

        result = command_move(src, dst_folder);
        free(src);
        free(dst_folder);
    } else if (strcmp(array[0], "rename") == 0) {
        char *src = fix_path(array[1], TRUE);
        char *new_name = array[2];

        result = command_rename(src, new_name);
        free(src);
    } else if (strcmp(array[0], "edit") == 0) {
        char *src = fix_path(array[1], TRUE);
        char *flag = array[2];
        char *content = array[3];

        result = command_edit(src, flag, content);
        free(src);
    } else if (strcmp(array[0], "permissions") == 0) {
        char *src = fix_path(array[1], TRUE);
        __mode_t permissions = parse_permission(array[2]);
        unsigned int recursive = (unsigned char) array[3][0];

        result = command_permissions(src, permissions, recursive);
        free(src);
    }

    for (int i = 0; i < n; ++i)
        free(array[i]);
    free(array);

    return result;
}