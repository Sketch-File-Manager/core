#ifndef SKETCH_CORE_SESSION_EDIT_H
#define SKETCH_CORE_SESSION_EDIT_H 1

/**
 * Exits the current session.
 */
extern void session_command_exit();

/**
 * Remove the last command from the queue.
 */
extern void session_command_undo();

/**
 * Add mkdir command to the queue.
 * @param dst The destination where the folder will be created.
 * @param folder The folder's name.
 * @param permissions The permissions number (e.x. 0700).
 */
extern void session_command_mkdir(char* dst, char* folder, char* permissions);

/**
 * Add create an empty file command to the queue.
 * @param dst The destination where the file will be created.
 * @param file The file's name
 * @param permissions The permissions number (e.x. 0700).
*/
extern void session_command_mkfile(char* dst, char* file, char* permissions);

/**
 * Add cp command to the queue.
 * @param src The source folder or file.
 * @param dst The destination folder where the folder or file will be copied.
*/
extern void session_command_copy(char* src, char* dst);

/**
 * Add mv command to the queue (without the rename function).
 * @param src The source folder or file.
 * @param dst The destination folder where the folder or file will be moved.
*/
extern void session_command_move(char* src, char* dst);

/**
 * Add renames a file or folder command to the queue.
 * @param src The source folder or file.
 * @param name The new name of the folder or file that will be renamed.
 */
extern void session_command_rename(char* src, char* name);

/**
 * Add edit file command to the queue.
 * @param src The source file that will be edited.
 * @param content The content that will be added.
 * @param flag -a or --append_to for appending to the end of the file.
 *             -u or --unshift for appending at the start of the file.
 *             -w or --write for discarding old content and write the new.
 */
extern void session_command_edit(char* src, char* flag, char* content);

/**
 * Change permissions in a folder or file.
 * @param src The folder or file path
 * @param permissions The permissions number (e.x. 0700).
 * @param recursive If target is folder then recursive will change all the permissions in sub files and sub folders. 0 for false, 1 for true.
 */
extern void session_command_permission(char* src, char* permissions, char* recursive);

#endif
