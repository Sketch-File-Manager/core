#ifndef SKETCH_CORE_SESSION_EDIT_H
#define SKETCH_CORE_SESSION_EDIT_H 1

/**
 * Exits the current session.
 */
extern void command_exit();

/**
 * Remove the last command from the queue.
 */
extern void command_undo();

/**
 * Add mkdir command to the queue.
 * @param dst The destination where the folder will be created.
 * @param folder The folder's name.
 */
extern void command_mkdir(char* dst, char* folder);

/**
 * Add create an empty file command to the queue.
 * @param dst The destination where the file will be created.
 * @param file The file's name
 */
extern void command_mkfile(char* dst, char* file);

/**
 * Add cp command to the queue.
 * @param src The source folder or file.
 * @param dst The destination folder where the folder or file will be copied.
 */
extern void command_copy(char* src, char* dst);

/**
 * Add mv command to the queue (without the rename function).
 * @param src The source folder or file.
 * @param dst The destination folder where the folder or file will be moved.
 */
extern void command_move(char* src, char* dst);

/**
 * Add renames a file or folder command to the queue.
 * @param src The source folder or file.
 * @param name The new name of the folder or file that will be renamed.
 */
extern void command_rename(char* src, char* name);

/**
 * Add edit file command to the queue.
 * @param src The source file that will be edited.
 * @param content The content that will be added.
 * @param flag -a or --append for appending to the end of the file.
 *             -u or --unshift for appending at the start of the file.
 *             -w or --write for discarding old content and write the new.
 */
extern void command_edit(char* src, char* flag, char* content);

#endif
