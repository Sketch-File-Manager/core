/* file_handler.h */
#ifndef SKETCH_CORE_FILE_HANDLER_H
#define SKETCH_CORE_FILE_HANDLER_H

#include <stdlib.h>
#include <codes.h>
#include <dirent.h>
#include <functions.h>
#include <stdbool.h>

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
    ino_t f_serial_number;

    // Link count
    nlink_t f_link_count;

    // File size (bytes)
    off_t f_size;
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
extern int list_files_names(const char *path, char ***result_files, size_t size);

extern int copy_with_byte_rate(const char *src, const char *dst, size_t rate);

/**
 * Find the size of a specific file.
 * @param path The path of the file of interest.
 * @return On success returns the size of the corespondent file.
 * On error returns -1.
 */
static inline size_t get_file_size(const char *path)
{
    struct stat file_stat;
    if (lstat(path, &file_stat) == -1) return FAILED;

    return file_stat.st_size;
}

/**
 * Checks whether an element is file/
 * @param dir_info The information of dir.
 * @param file_stat The information current file/dir.
 * @return On success returns 1. On error returns 0.
 */
static inline bool is_file(struct dirent *dir_info,
                           struct stat   *file_stat)
{
    if ( !(!strcmp(dir_info->d_name, ".") | !strcmp(dir_info->d_name, "..")) &&
        !(file_stat->st_mode & S_IFDIR))
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Finds the number of files in a specific directory.
 * @param path The directory to search.
 * @return On success returns the number of files.
 * On error returns -1.
 */
static inline size_t get_files_number(const char *path)
{
    DIR *dir = opendir(path);
    struct dirent *dir_info;
    struct stat    file_stat;

    if (dir == NULL || lstat(path, &file_stat) == -1) return FAILED;

    size_t files_s = 0;
    while ((dir_info = readdir(dir)) != NULL)
        if (is_file(dir_info, &file_stat))
            ++files_s;

    closedir(dir);
    return files_s;
}


#endif
