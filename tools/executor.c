#include <string.h>
#include <include/codes.h>
#include <include/functions.h>
#include <stdlib.h>

#include "executor.h"
#include "../commands/commands.h"

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
    char** array = split_except(command, ' ', '\\', &n);

    if(strcmp(array[0], "mkdir") == 0) {
        char* dst_folder = fix_path(array[1], TRUE);
        char* name = array[2];
        __mode_t permissions = parse_permission(array[3]);

        return command_mkdir(dst_folder, name, permissions);
    }
    else if(strcmp(array[0], "mkfile") == 0) {
        char* dst_folder = fix_path(array[1], TRUE);
        char* file = array[2];
        __mode_t permissions = parse_permission(array[3]);

        return command_mkfile(dst_folder, file, permissions);
    }
    else if(strcmp(array[0], "copy") == 0) {
        char* src = fix_path(array[1], TRUE);
        char* dst_folder = fix_path(array[2], TRUE);

        return command_copy(src, dst_folder);
    }
    else if(strcmp(array[0], "move") == 0) {
        char* src = fix_path(array[1], TRUE);
        char* dst_folder = fix_path(array[2], TRUE);

        return command_move(src, dst_folder);
    }
    else if(strcmp(array[0], "rename") == 0) {
        char* src = fix_path(array[1], TRUE);
        char* new_name = array[2];

        return command_rename(src, new_name);
    }
    else if(strcmp(array[0], "edit") == 0) {
        char* src = fix_path(array[1], TRUE);
        char* content = array[2];
        char* flag = array[3];

        return command_edit(src, content, flag);
    }
    else if(strcmp(array[0], "permissions") == 0) {
        char* src = fix_path(array[1], TRUE);
        __mode_t permissions = parse_permission(array[2]);
        unsigned int recursive = (unsigned char) array[3][0];

        return command_permissions(src, permissions, recursive);
    }

    for (int i = 0; i < n; ++i)
        free(array[i]);
    free(array);

    return UNKNOWN_COMMAND;
}