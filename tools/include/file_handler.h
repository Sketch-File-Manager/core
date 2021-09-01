#ifndef SKETCH_CORE_FILE_HANDLER_H
#define SKETCH_CORE_FILE_HANDLER_H 1

#include <stdlib.h>

int read_file(char *file_path, char **config_content);

int write_file(char *file_path, char *changes, size_t changes_len);

#endif
