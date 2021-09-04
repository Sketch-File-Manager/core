#ifndef SESSION_PARSER_H
#define SESSION_PARSER_H 1

#include <stdlib.h>

extern int delete_file(const char *name);

extern int create_file(const char *name);

extern int delete_last_line(const char *name);

extern int append_to_end(const char *name, const char *content);

extern int append_to_start(const char *name, const char *content);

extern int read_session(const char *name, char ***result, size_t *size);

extern int session_exists(const char* name);

extern int list_sessions(char ***result, size_t *size);

#endif