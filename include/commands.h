#ifndef CORE_COMMANDS_H
#define CORE_COMMANDS_H

#include <bits/types.h>

/**
 * Create a new directory at destination.
 * @param dst The destination of the new directory.
 * @param name The name of the new directory.
 * @param perms The permissions for the new directory.
 * @return 0 on success.
 */
extern int command_mkdir(const char *dst, const char *name, __mode_t perms);

/**
 * Create a new file at destination.
 * @param dst The destination of the the new file.
 * @param name The name of the new file.
 * @param perms The permissions for the new file.
 * @return 0 on success
 */
extern int command_mkfile(const char *dst, const char *name, __mode_t perms);

/**
 * Copy the source file or directory at the destination directory
 * and preserves the filename and permissions.
 * @param dst The destination where the file or directory will be copied.
 * @param src_path The source file or directory that will be copied.
 * @return 0 on success.
 */
extern int command_copy(const char *dst, const char *src, unsigned int byte_rate);

/**
 * Moves the source file or directory at the destination directory
 * and preserves the filename and permissions.
 * @param dst The destination where the file or directory will be moved.
 * @param src_path The source file or directory that will be moved.
 * @return 0 on success.
 */
extern int command_move(const char *dst, const char *src_path, unsigned int byte_rate);

/**
 * Renames a file or directory and preserves the permissions.
 * @param new_name The new name of the file or directory.
 * @param src The file or directory that will be renamed.
 * @return 0 on success.
 */
extern int command_rename(const char *new_name, const char *src);

/**
 * Change the permissions of a file or directory.
 * @param new_perms The new permissions
 * @param src The file or directory that the permissions will be applied.
 * @return 0 on success.
 */
extern int command_permissions(__mode_t new_perms, const char *src);

/**
 * Change the permissions of a directory and its children recursively.
 * @param new_perms The new permissions.
 * @param new_perms The new permissions.
 * @param depth On 0 will change all children recursively. On 1 will change only the parent folder.
 * By adding 1 it will change recursively the folder and files below.
 * @return 0 on success.
 */
extern int command_permissions_recursive(__mode_t new_perms, const char *src, unsigned int depth);

/**
 * List the files of a directory. This will not work for session changes.
 * @param src The directory that will be listed.
 * @return 0 on success.
 */
extern int command_list(const char *src);

/**
 * Deletes a file or directory.
 * @param src
 * @return
 */
extern int command_delete(const char *src);

#endif //CORE_COMMANDS_H
