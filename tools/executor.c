#include <string.h>
#include <stdio.h>
#include <codes.h>
#include <sys/stat.h>
#include <functions.h>
#include <stdlib.h>
#include <unistd.h>
#include "executor.h"

static char* fix_path(char* path) {
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
    if(endsWith(ret, "/") == FALSE) {
        char* tmp = calloc(strlen(ret) + 2, sizeof(char));
        strcpy(tmp, ret);
        strcat(tmp, "/");
        ret = tmp;
    }

    return ret;
}

int execute(char* command) {
    char* cmd = strtok(command, " ");

    if(strcmp(cmd, "mkdir") == 0) {
        char* path = fix_path(strtok(NULL, " "));
        char* name = strtok(NULL, " ");

        char* folder_path = calloc(strlen(path) + strlen(name) + 1, sizeof(char));
        strcpy(folder_path, path);
        strcpy(folder_path, name);
        mkdir(folder_path, 0700);
    }
    else if(strcmp(cmd, "mkfile") == 0) {

    }
    else if(strcmp(cmd, "copy") == 0) {

    }
    else if(strcmp(cmd, "move") == 0) {

    }
    else if(strcmp(cmd, "rename") == 0) {

    }
    else if(strcmp(cmd, "edit") == 0) {

    }

    return SUCCESS;
}