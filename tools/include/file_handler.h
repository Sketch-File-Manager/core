/* file_handler.h */
#ifndef SKETCH_CORE_FILE_HANDLER_H
#define SKETCH_CORE_FILE_HANDLER_H

#include <stdlib.h>

typedef struct file_info {
    // Basic info of file.
    char *f_name;
    mode_t f_permissions;
    uid_t f_user_id;
    gid_t f_group_id;
    // Times.
    struct timespec f_last_access;
    struct timespec f_last_modify;
    struct timespec f_status_change;

    // File serial number
    __ino_t f_serial_number;

    // Link count
    __nlink_t f_link_count;

    // File size (bytes)
    __off_t f_size;
} file_info;

extern int get_info_of(char *path, file_info ***files, size_t *size);

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

/**
 * Lists the all the files and sub folder of a  directory.
 * @param path The folder path,
 * @param result_files An array of string names.
 * @param size The size of the array.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int list_files_names(const char *path, char ***result_files, size_t *size);

extern int copy_with_byte_rate(const char *src, const char *dst, size_t rate);

#endif
