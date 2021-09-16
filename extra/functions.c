#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include "include/functions.h"
#include <include/codes.h>
#include <sys/stat.h>
#include <dirent.h>

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

int startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : memcmp(pre, str, lenpre) == 0;
}

char* append(const char* str1, const char* str2) {
    char* ret = calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
    strcpy(ret, str1);
    strcat(ret, str2);
    return ret;
}

char* get_home_path() {
    char *username = getlogin();
    size_t path_s = strlen("/home/") + strlen(username) + 1 + 1;

    char* path = calloc(path_s, sizeof(char));
    strcpy(path, "/home/");
    strcat(path, username);
    strcat(path, "/");

    return path;
}

char* fix_path(char* path, int add_slash) {
    char *ret = calloc(strlen(path) + 1, sizeof(char));
    strcpy(ret, path);

    // If starts with ~ replace with /home/username
    if(ret[0] == '~'){
        ret = ret + 2; // Delete ~/
        ret = append(get_home_path(), ret);
    }

    // Ends with /
    if(add_slash == TRUE && endsWith(ret, "/") == FALSE)
        ret = append(ret, "/");

    return ret;
}

char *merge_home_relative_filename(const char *filename, const char *relative_path) {
    // Remove /
    if(startsWith(relative_path, "/") == TRUE)
        relative_path += 1;

    return append(get_home_path(), append(relative_path, filename));
}

char** split(char* str, char delimiter, char prev_delim_except, size_t* n) {
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

int is_dir(const char *path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return FALSE;

    // do an and statement with bits of st_mode and bits of S_IFDIR.
    // If is the same the result is ok, otherwise the result is zero.
    if (path_stat.st_mode & S_IFDIR)
        return TRUE;

    return FALSE;
}

void read_contents_of(const char *path, queue *c_queue) {
    DIR *dir = opendir(path);
    struct dirent *dir_contents = NULL;
    char *tmp_path;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL) {
        // Save the path.
        tmp_path = calloc(strlen(path) + strlen(dir_contents->d_name) + 1, sizeof(char));
        strcpy(tmp_path, path);
        strcat(tmp_path, dir_contents->d_name);

        // Add it to the queue.
        add(c_queue, tmp_path);
    }
}