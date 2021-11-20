/* session_parser.h */
#ifndef SESSION_PARSER_H
#define SESSION_PARSER_H 1

#include <stdlib.h>

/**
 * Deletes a session.
 * @param name The session's name.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int delete_file(const char *name);

/**
 * Creates a new session.
 * @param name The session name.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int create_file(const char *name);

/**
 * Undo the changes that applied last.
 * @param name The session's name.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int delete_last_line(const char *name);

/**
 * Adds a new command to the session file.
 * @param name The session's name.
 * @param content The command.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int append_to_end(const char *name, const char *content);

/**
 * Marks a session as executed .
 * @param name The session's name.
 * @param content The "executed" text.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int append_to_start(const char *name, const char *content);

/**
 * Read all the commands of a session.
 * @param name The session's name.
 * @param result The string array where the commands will be stored.
 * @param size The size of the array.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int read_session(const char *name, char ***result, size_t *size);

/**
 * Checks if a session exists.
 * @param name The session's name.
 * @return TRUE or FALSE if the session exist or not.
 */
extern int session_exists(const char *name);

/**
 * Retrieves all the active sessions' names.
 * @param result The array with the sessions' names.
 * @param size The size of the array.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int list_sessions(char ***result, size_t *size);

#endif