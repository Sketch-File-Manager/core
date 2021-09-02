#ifndef SESSION_PARSER_H
#define SESSION_PARSER_H 1

#include <stdlib.h>

extern int delete_file(char *name);

extern int create_file(char *name);

extern int delete_last_line(char *name);

extern int append_to_end(char *name, char *content);

extern int append_to_start(char *name, char *content);

extern int read_session(char *name, char ***result, size_t *size);

extern int session_exists(char* name);

extern int list_sessions(char ***result, size_t *size);

#endif