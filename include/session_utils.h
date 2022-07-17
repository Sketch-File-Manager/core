#ifndef SKETCH_CORE_SESSION_UTILS_H
#define SKETCH_CORE_SESSION_UTILS_H

#include <bits/types.h>
#include <stdlib.h>

/**
 * Add the command mkdir to the session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_mkdir(const char *session, const char *dst, const char *name, __mode_t perms);

/**
 * Add the command mkfile to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_mkfile(const char *session, const char *dst, const char *name, __mode_t perms);

/**
 * Add the command copy to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_copy(const char *session, const char *dst, const char *src_path);

/**
 * Add the command move to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_move(const char *session, const char *dst, const char *src_path);

/**
 * Add the command rename to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_rename(const char *session, const char *new_name, const char *src);

/**
 * Add the command permissions to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_permissions(const char *session, __mode_t new_perms, const char *src);

/**
 * Add the command permissions-recursive to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_permissions_recursive(const char *session, __mode_t new_perms, const char *src, unsigned int depth);

/**
 * Virtualize the session file and list the directory's contents.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_list(const char *session, const char *src);

/**
 * Add the command delete to the end of session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_delete(const char *session, const char *src);

/**
 * Undo the last command that was added to the session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern int session_undo(const char *session);

/**
 * Read the session file.
 * @param dst The sessions files contents.
 * @param src The session file's path.
 * @return The number of commands that the session file has.
 */
extern size_t session_read_session(char *dst, const char *src);

/**
 * Delete the session file.
 * @param session The session file.
 * @return 0 on success.
 */
extern void session_delete_session(const char *session);

#endif //SKETCH_CORE_SESSION_UTILS_H
