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
    char        *f_name;
    struct stat *f_stats;
} file_info;

extern int get_files_info(const char *path, file_info *results, size_t files);

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
extern int list_files_names(const char *path, char **result_files, size_t size);

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
 * @param file_name The name of the current file.
 * @param path  The path of the current file.
 * @return On success returns 1. On error returns 0.
 */
static inline bool is_file(struct dirent *dir_info, const char *file_name,
                           const char *path)
{
    size_t absolute_path_s = strlen(file_name) + strlen(path) + 1;
    char absolute_path[absolute_path_s];
    memset(absolute_path, 0x0, absolute_path_s);
    strcpy(absolute_path, path);
    strcat(absolute_path, file_name);

    struct stat file_stat;
    if (lstat(absolute_path, &file_stat) == -1) return FALSE;

    if ( !(!strcmp(dir_info->d_name, ".") | !strcmp(dir_info->d_name, "..")) &&
        !(file_stat.st_mode & S_IFDIR))
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
        if (is_file(dir_info, dir_info->d_name, path))
            ++files_s;

    closedir(dir);
    return files_s;
}


#endif
