/* functions.h */
#ifndef SKETCH_CORE_FUNCTIONS_H
#define SKETCH_CORE_FUNCTIONS_H


#include <include/queue.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include <include/codes.h>

/**
 * Checks if a string ends with another string.
 * @param str The base string.
 * @param suffix The possible ending string.
 * @return TRUE or FALSE if it ends with or not.
 */
extern int endsWith(const char *str, const char *suffix);

/**
 * Return the concat of two strings.
 * @param str1 The str1 string
 * @param str2 The str2 string.
 * @return The concat of the the two strings.
 */
extern char *str_add(const char *str1, ...);

/**
 * Return the home directory.
 * @return The home directory.
 */
static inline char *get_home_path() {
    return str_add("/home/", getlogin(), "/", NULL);
}

/**
 * Replace ~ with home directory and add / at the end if missing.
 */
extern char *fix_path(const char *path, int add_slash);

/**
 * Splits a string to an array of strings by a specific delimiter. It will skip the split_except by delimiter if the previous character is the same as the prev_delim_except.
 * @param str The string that will be split_except.
 * @param delimiter The delimiter.
 * @param prev_delim_except The exception character that is located before the delimiter.
 * @param n The size of the array that will be returned.
 * @return A string array that contains the split_except form of the str.
 */
extern char **split_except(char *str, char delimiter, char prev_delim_except, size_t *n);

/**
 * Checks if a path is a directory.
 * @param path The path to check.
 * @return TRUE if the path is directory, FALSE otherwise.
 */

static inline int is_dir(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) return FALSE;

    if (path_stat.st_mode & S_IFDIR) return TRUE;

    return FALSE;
}



/**
 * Reads all the contents of a directory and appends them to a given queue.
 * @param directory The directory that will be searched.
 * @param c_queue The queue where the information will be saved.
 */
extern void read_contents_of(const char *directory, queue *c_queue);

/**
 * Returns the permissions of a file or directory.
 * @param src The file or directory.
 * @return The permissions.
 */

static inline mode_t get_permissions_of(const char *src) {
    struct stat path_stat;
    if (stat(src, &path_stat) == -1) return -1;

    return path_stat.st_mode;
}

/**
 * Generates a random string of a specific size.
 * @param size The size the random string will have.
 * @return The random generated string.
 */
extern char *rand_string(size_t size);

/**
 * Appends a backslash (\) before every space in a string
 * @param str The string that will be analyzed.
 * @return The analyzed string.
 */
extern char *analyze_string_spaces(char *str);

static inline char *to_string(int number) {
    static char result[10]; // Max 2 ** 32 - 1
    sprintf(result, "%d", number);

    return result;
}


#endif
