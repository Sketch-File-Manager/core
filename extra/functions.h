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



#endif
