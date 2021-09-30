#ifndef SKETCH_CORE_COMMANDS_H
#define SKETCH_CORE_COMMANDS_H

#include <bits/types.h>

/**
 * Make a new directory.
 * @param dst_folder The destination folder.
 * @param name The new directory's name.
 * @param permissions The permissions for this directory.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_mkdir(char *dst_folder, char *name, __mode_t permissions);

/**
 * Make a new file.
 * @param dst_folder The destination folder.
 * @param name The file's name.
 * @param permissions The permissions for this file.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_mkfile(char *dst_folder, char *name, __mode_t permissions);

/**
 * Copy a file or directory inside another directory.
 * @param src The file or directory that will be copied.
 * @param dst_folder The destination folder where it will be copied.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_copy(char *src, char *dst_folder);

/**
 * Move a file or directory inside another directory.
 * @param src The file or directory that will be moved.
 * @param dst_folder The destination folder where it will be moved.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_move(char *src, char *dst_folder);

/**
 * Rename a file or directory.
 * @param src The file ot directory that will be renamed.
 * @param new_name The new name for the file or directory.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_rename(char *src, char *new_name);

/**
 * Edit an existing file.
 * @param src The file that will be edited.
 * @param flag -a --append for adding to the bottom. -u --unshift to adding to the start. -w --write for discarding old content.
 * @param content The contents that will be added.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_edit(char *src, char *flag, char *content);

/**
 * Change the permissions of file or directory.
 * @param src The file or directory that the permissions will be changed.
 * @param permissions The new permissions, e.x. 0755.
 * @param recursive If 'src' is a folder the recursive will change the permissions of all the sub files and directories. Can accept 0 and 1.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_permissions(char *src, __mode_t permissions, unsigned int recursive);

/**
 * Lists to stdout a json file with all the files and directories inside a directory and their information.
 * @param directory The directory where the command will be applied.
 * @return SUCCESS or the appropriate error number.
 */
extern int command_ls(char *directory);

#endif //SKETCH_CORE_COMMANDS_H
