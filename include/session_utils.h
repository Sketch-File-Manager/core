#ifndef SKETCH_CORE_SESSION_UTILS_H
#define SKETCH_CORE_SESSION_UTILS_H

#include <bits/types.h>
#include <stdlib.h>

/**
 * Add the command mkdir to the session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_mkdir(FILE *session, const char *dst, const char *name, const char *perms);

/**
 * Add the command mkfile to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_mkfile(FILE *session, const char *dst, const char *name, const char *perms);

/**
 * Add the command copy to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_copy(FILE *session, const char *dst, const char *src);

/**
 * Add the command move to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_move(FILE *session, const char *dst, const char *src);

/**
 * Add the command rename to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_rename(FILE *session, const char *new_name, const char *src);

/**
 * Add the command permissions to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_permissions(FILE *session, const char *new_perms, const char *src);

/**
 * Add the command permissions-recursive to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_permissions_recursive(FILE *session, const char *new_perms, const char *src, unsigned int depth);

/**
 * Virtualize the session file and list the directory's contents.
 * @param session The session filename.
 * @return 0 on success.
 */
extern int session_list(const char *session, const char *src);

/**
 * Add the command delete to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_delete(FILE *session, const char *src);

/**
 * Undo the last command that was added to the session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_undo(FILE *session, char *session_filename);

/**
 * Executes the current session
 * @param session The session filename.
 * @keep To keep or delete the session after execution.
 * @return 0 on success.
 */
extern int session_execute(const char *session, int keep);

#endif //SKETCH_CORE_SESSION_UTILS_H
