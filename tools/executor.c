#include <string.h>
#include <stdio.h>
#include <codes.h>
#include <sys/stat.h>
#include <functions.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <session_parser.h>
#include <dirent.h>
#include "executor.h"


typedef struct perm_queue {
    char   *p_path;
    int     p_is_dir;
} perm_queue;


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

static void read_contents_of(const char *path, perm_queue **queue, size_t queue_s) {
    DIR *dir = NULL;
    struct dirent *dir_contents = NULL;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL) {
        // Save the path.
        queue[queue_s - 1] = calloc(1, sizeof(perm_queue));
        queue[queue_s - 1]->p_path = calloc(strlen(path) + strlen(dir_contents->d_name) + 1, sizeof(char));
        strcpy(queue[queue_s - 1]->p_path, path);
        strcat(queue[queue_s - 1]->p_path, dir_contents->d_name);

        // Set True if the current content is folder.
        if (dir_contents->d_type != DT_REG)
            queue[queue_s - 1]->p_is_dir = TRUE;
        else
            queue[queue_s - 1]->p_is_dir = FALSE;

        // Increase the size of queue.
        queue = realloc(queue, ++queue_s);
    }
}

static void set_perm_recursive(const char *path, __mode_t perms) {

    size_t queue_s = 1;
    perm_queue **queue = calloc(1, sizeof(perm_queue *));
    char *tmp;
    // Read the first contents.
    read_contents_of(path, queue, queue_s);

    while (queue != NULL) {
        chmod(queue[queue_s - 1]->p_path, perms);

        // temporary save the current dir.
        if (queue[queue_s - 1]->p_is_dir == TRUE) {
            tmp = calloc(strlen(queue[queue_s - 1]->p_path) + 1, sizeof(char));
            strcpy(tmp, queue[queue_s - 1]->p_path);
        }

        // free the current dir.
        free(queue[queue_s - 1]->p_path);
        free(queue[queue_s - 1]);

        // decrease the size of the queue ( remove the last element ).
        queue = realloc(queue, --queue_s);

        if (queue[queue_s - 1]->p_is_dir == TRUE)
            read_contents_of(tmp, queue, queue_s);

    }

    free(queue);
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
        char* dst_folder = fix_path(split[1], TRUE);
        char* name = split[2];
        __mode_t permissions = parse_permission(split[3]);

        char* folder_path = calloc(strlen(dst_folder) + strlen(name) + 1, sizeof(char));
        strcpy(folder_path, dst_folder);
        strcpy(folder_path, name);

        int result = mkdir(folder_path, permissions);
        if(result == 0)
            return SUCCESS;

        return errno;
    }
    else if(strcmp(split[0], "mkfile") == 0) {
        char* dst_folder = fix_path(split[1], TRUE);
        char* file = split[2];
        __mode_t permissions = parse_permission(split[3]);

        int result = create_file(file);
        if (result == 0)
            return SUCCESS;

        return FALSE;
    }
    else if(strcmp(split[0], "copy") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* dst_folder = fix_path(split[2], TRUE);
        __mode_t permissions = parse_permission(split[3]);
        unsigned int recursive = (unsigned char) split[4][0];



    }
    else if(strcmp(split[0], "move") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* dst_folder = fix_path(split[2], TRUE);
        __mode_t permissions = parse_permission(split[3]);
        unsigned int recursive = (unsigned char) split[4][0];


    }
    else if(strcmp(split[0], "rename") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* new_name = split[2];


    }
    else if(strcmp(split[0], "edit") == 0) {
        char* src = fix_path(split[1], TRUE);
        char* content = split[2];
        char* flag = split[3];


    }
    else if(strcmp(split[0], "permissions") == 0) {
        char* src = fix_path(split[1], TRUE);
        __mode_t permissions = parse_permission(split[2]);
        unsigned int recursive = (unsigned char) split[3][0];
        

    }

    for (int i = 0; i < n; ++i)
        free(split[i]);
    free(split);

    return SUCCESS;
}