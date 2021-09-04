#include <string.h>
#include <stdio.h>
#include <codes.h>
#include <sys/stat.h>
#include <functions.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include "executor.h"

/**
 * Replace ~ with home directory and add / at the end if missing.
 */
static char* fix_path(char* path, int add_slash) {
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

static char** split_with_exception(char* str, char delimiter, char prev_delim_except, size_t* n) {
    char** ret = (char**) calloc(1, sizeof (char*));
    int a = 0;

    char* token = calloc(strlen(str) + 1, sizeof(char));
    int k = 0;
    for (int i = 0; str[i]; i++) {
        if(str[i] != delimiter || (i > 0 && str[i - 1] != prev_delim_except)) {
            token[k] = str[i];
            k++;
            continue;
        }

        ret[a] = calloc(strlen(token) + 1, sizeof(char));
        strcpy(ret[a], token);
        ret = realloc(ret, (a + 1) + 1);
        a++;

        token = "";
        k = 0;
    }

    *n = a + 1;
    free(token);
    return ret;
}

static __mode_t parse_permission(char* p) {
    return (__mode_t) p;
}

int execute(char* command) {
    size_t n;
    char** split = split_with_exception(command, ' ', '\\', &n);

    if(strcmp(split[0], "mkdir") == 0) {
        char* path = fix_path(split[1], TRUE);
        char* name = split[2];
        __mode_t permissions = parse_permission(split[3]);

        char* folder_path = calloc(strlen(path) + strlen(name) + 1, sizeof(char));
        strcpy(folder_path, path);
        strcpy(folder_path, name);

        int result = mkdir(folder_path, permissions);
        if(result == 0)
            return SUCCESS;

        return errno;
    }
    else if(strcmp(split[0], "mkfile") == 0) {
        
    }
    else if(strcmp(split[0], "copy") == 0) {

    }
    else if(strcmp(split[0], "move") == 0) {

    }
    else if(strcmp(split[0], "rename") == 0) {

    }
    else if(strcmp(split[0], "edit") == 0) {

    }
    else if(strcmp(split[0], "permissions") == 0) {

    }

    for (int i = 0; i < n; ++i)
        free(split[i]);
    free(split);

    return SUCCESS;
}