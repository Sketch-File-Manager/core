#ifndef SKETCH_CORE_COMMANDS_H
#define SKETCH_CORE_COMMANDS_H

#include <bits/types.h>

extern int command_mkdir(char *dst_folder, char *name, __mode_t permissions);

extern int command_mkfile(char *dst_folder, char *name, __mode_t permissions);

extern int command_copy(char *src, char *dst_folder);

extern int command_move(char *src, char *dst_folder);

extern int command_rename(char *src, char *new_name);

extern int command_edit(char *src, char *content, char *flag);

extern int command_permissions(char *src, __mode_t permissions, unsigned int recursive);

extern int command_ls(char *directory);

#endif //SKETCH_CORE_COMMANDS_H
