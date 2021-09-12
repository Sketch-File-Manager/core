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


char* fix_path(char* path, int add_slash) {
    char *ret = calloc(strlen(path) + 1, sizeof(char));
    strcpy(ret, path);

    // If starts with ~ replace with /home/username
    if(ret[0] == '~'){
        ret = ret + 1;
        char *username = getlogin();

        size_t path_s = strlen("/home/") + strlen(username) + strlen(ret);
        char* home_path = calloc(path_s, sizeof(char));
        strcpy(home_path, "/home/");
        strcpy(home_path, username);
        strcpy(home_path, ret);

        ret = home_path;
    }

    // Ends with /
    if(add_slash == TRUE && endsWith(ret, "/") == FALSE) {
        char* tmp = calloc(strlen(ret) + 2, sizeof(char));
        strcpy(tmp, ret);
        strcat(tmp, "/");
        ret = tmp;
    }

    return ret;
}

char *merge_home_relative_filename(const char *filename, const char *relative_path) {
    char *username = getlogin();
    size_t absolute_path_s = strlen("/home/") + strlen(username) + strlen(filename) + strlen(relative_path);
    char *absolute_path = calloc(absolute_path_s + 2, sizeof(char));

    strcpy(absolute_path, "/home/");
    strcat(absolute_path, username);
    strcat(absolute_path, relative_path);
    strcat(absolute_path, filename);

    return absolute_path;
}