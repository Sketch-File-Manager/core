//
// Created by george on 9/2/21.
//

#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include "functions.h"
#include <codes.h>

int endsWith(const char *str, const char *suffix) {
    if (!str || !suffix)
        return FALSE;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return FALSE;

    if(strncmp(str + lenstr - lensuffix, suffix, lensuffix) != 0)
        return FALSE;

    return TRUE;
}

char *get_absolute_path(const char *name, const char *relative_path) {
    char *username = getlogin();
    size_t absolute_path_s = strlen("/home/") + strlen(username) + strlen(name) + strlen(relative_path);
    char *absolute_path = calloc(absolute_path_s + 2, sizeof(char));

    strcpy(absolute_path, "/home/");
    strcat(absolute_path, username);
    strcat(absolute_path, relative_path);
    strcat(absolute_path, name);

    return absolute_path;
}