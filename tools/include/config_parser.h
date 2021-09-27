#ifndef SKETCH_CORE_CONFIG_PARSER_H
#define SKETCH_CORE_CONFIG_PARSER_H 1

/**
 * Return the current session file.
 * @param current The variable where the string name will be stored.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int get_current(char **current);

/**
 * Sets the current session file.
 * @param current The session name
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int set_current(const char *current);

#endif