#ifndef SKETCH_CORE_FILE_HANDLER_H
#define SKETCH_CORE_FILE_HANDLER_H 1

#include <stdlib.h>

/**
 * Read a files content.
 * @param file_path The file path
 * @param config_content The return pointer that will contain the content.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int read_file(const char *file_path, char **config_content);

/**
 * Write a string to a file.
 * @param file_path The file that will be written.
 * @param changes The content that will be added.
 * @param changes_len The length of the content.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int write_file(const char *file_path, char *changes, size_t changes_len);

extern void check_requirements(const char *config_file, const char *sketch_folder_name, const char *sessions_folder_name);

/**
 * Lists the all the files and sub folder of a  directory.
 * @param path The folder path,
 * @param result_files An array of string names.
 * @param size The size of the array.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int list_files(const char *path, char ***result_files, size_t *size);

#endif
