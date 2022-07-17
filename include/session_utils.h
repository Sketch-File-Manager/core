#ifndef SKETCH_CORE_SESSION_UTILS_H
#define SKETCH_CORE_SESSION_UTILS_H

#include <bits/types.h>
#include <stdlib.h>

extern int session_mkdir(const char *dst, const char *name, __mode_t perms);

extern int session_mkfile(const char *dst, const char *name, __mode_t perms);

extern int session_copy(const char *dst, const char *src_path);

extern int session_move(const char *dst, const char *src_path);

extern int session_rename(const char *new_name, const char *src);

extern int session_permissions(__mode_t new_perms, const char *src);

extern int session_permissions_recursive(__mode_t new_perms, const char *src, int depth);

extern int session_list(const char *src);

extern int session_delete(const char *src);

/**
 * Read the session file.
 * @param dst The sessions files contents.
 * @param src The session file's path.
 * @return The number of commands that the session file has.
 */
extern size_t session_read_session(char *dst, const char *src);

extern void session_delete_session(const char *src);




#endif //SKETCH_CORE_SESSION_UTILS_H