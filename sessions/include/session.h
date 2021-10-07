/* session.h */
#ifndef SKETCH_CORE_SESSION_H
#define SKETCH_CORE_SESSION_H

/**
 * Starts a new session with a random id.
 */
extern void session_start();

/**
 * Deletes a session with the specified id.
 * @param id The session's id.
 */
extern void session_end(char *id);

/**
 * Set a session as current so it will be available in session edit commands.
 * @param id The session's id.
 */
extern void session_use(char *id);

/**
 * Execute the commands of a session. After it's execution the session will not be available to be executed again.
 * If one or more commands cannot be executed then the rest of the session will be discarded.
 * @param id The session's id.
 */
extern void session_run(char *id);

/**
 * Prints the current session's id.
 */
extern void session_current();

/**
 * Prints a session content based on id.
 * @param id The session's id.
 */
extern void session_show(char *id);

/**
 * List all the sessions
 */
extern void session_list();

#endif
