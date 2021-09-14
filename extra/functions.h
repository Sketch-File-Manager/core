#ifndef SKETCH_CORE_FUNCTIONS_H
#define SKETCH_CORE_FUNCTIONS_H 1

/**
 * Checks if a string ends with another string.
 * @param str The base string.
 * @param suffix The possible ending string.
 * @return TRUE or FALSE if it ends or not.
 */
extern int endsWith(const char *str, const char *suffix);

/**
 * Replace ~ with home directory and add / at the end if missing.
 */
extern char* fix_path(char* path, int add_slash);

/**
 * Returns the path with the home directory at the start.
 * @param filename The file's filename.
 * @param relative_path The relative path.
 * @return The fixed path.
 */
extern char *merge_home_relative_filename(const char *filename, const char *relative_path);

/**
 * Splits a string to an array of strings by a specific delimiter. It will skip the split by delimiter if the previous character is the same as the prev_delim_except.
 * @param str The string that will be split.
 * @param delimiter The delimiter.
 * @param prev_delim_except The exception character that is located before the delimiter.
 * @param n The size of the array that will be returned.
 * @return A string array that contains the split form of the str.
 */
extern char **split_with_exception(char* str, char delimiter, char prev_delim_except, size_t* n);


#endif
