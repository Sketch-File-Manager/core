#ifndef SKETCH_CORE_SESSION_UTILS_H
#define SKETCH_CORE_SESSION_UTILS_H

#include <bits/types.h>
#include <stdlib.h>

extern int session_mkdir(const char *session, const char *dst, const char *name, __mode_t perms);

extern int session_mkfile(const char *session, const char *dst, const char *name, __mode_t perms);

extern int session_copy(const char *session, const char *dst, const char *src_path);

extern int session_move(const char *session, const char *dst, const char *src_path);

extern int session_rename(const char *session, const char *new_name, const char *src);

extern int session_permissions(const char *session, __mode_t new_perms, const char *src);

extern int session_permissions_recursive(const char *session, __mode_t new_perms, const char *src, unsigned int depth);

extern int session_list(const char *session, const char *src);

extern int session_delete(const char *session, const char *src);

/**
 * Read the session file.
 * @param dst The sessions files contents.
 * @param src The session file's path.
 * @return The number of commands that the session file has.
 */
extern size_t session_read_session(char *dst, const char *src);

extern void session_delete_session(const char *session);

#endif //SKETCH_CORE_SESSION_UTILS_H
