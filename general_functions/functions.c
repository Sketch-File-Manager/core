/* functions.c */
#include <string.h>
#include <malloc.h>
#include <dirent.h>
#include <time.h>

#include <functions.h>
#include <mem.h>

int ends_with(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return FALSE;

    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return FALSE;

    if (strncmp(str + lenstr - lensuffix, suffix, lensuffix) != 0)
        return FALSE;

    return TRUE;
}

char *fix_path(const char *path, int add_slash)
{
    char *ret = NULL;
    size_t ret_s = 0;

    // If starts with ~ replace with /home/username
    if (path[0] == '~') ret_s = strlen(path) + strlen(getenv("HOME")) + add_slash;
    else ret_s = strlen(path) + add_slash;

    // Allocate space for the new string.
    ALLOCATE_MEM(ret, ret_s + 1, sizeof(char));
    // Build the new string.
    sprintf(ret, "%s%s", getenv("HOME"), path + 2);

    // Ends with /
    if (add_slash == TRUE &&
        ends_with(path, "/") == FALSE) sprintf(ret, "%s%c", ret, '/');

    return ret;
}

char **split_except(char *str, char delimiter, char prev_delim_except, size_t *n)
{
    // TODO - Reduce the allocations in this function.
    // TODO - Make this function better.
    char **ret;
    ALLOCATE_MEM(ret, 1, sizeof(char *));
    int a = 0;
    *n = 0;

    char *token;
    ALLOCATE_MEM(token, strlen(str) + 1, sizeof(char));
    int k = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != delimiter || (i > 0 && str[i - 1] == prev_delim_except))
        {
            token[k] = str[i];
            k++;
            continue;
        }

        sprintf(ret[a], "%s", token);
        a++;
        REALLOCATE_MEM(ret, (a + 1) * sizeof(char *))
        // Free the previous pointer.
        FREE_MEM(token)
        // Allocate space for new token.
        ALLOCATE_MEM(token, strlen(str) + 1, sizeof(char))
        k = 0;
    }

    sprintf(ret[a], "%s", token);

    *n = a + 1;
    FREE_MEM(token);
    return ret;
}

void read_contents_of(const char *directory, queue *c_queue)
{
    DIR *dir = opendir(directory);
    struct dirent *dir_contents = NULL;
    char *tmp_path = NULL;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL)
    {
        // Allocate space for the new item.
        ALLOCATE_MEM(tmp_path, strlen(directory) + strlen(dir_contents->d_name) + 1,
                     sizeof(char))
        // Save the directory.
        sprintf(tmp_path,  "%s%s", directory,
                dir_contents->d_name);
        // Add it to the queue.
        add(c_queue, tmp_path);
    }
    closedir(dir);
}

char *rand_string(size_t size) {
    srand(time(NULL));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    char *str;
    ALLOCATE_MEM(str, size + 1, sizeof(char));
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char *analyze_string_spaces(char *str) {
    int space_count = 0;
    // count spaces
    for (int i = 0; str[i]; i++, str[i] == ' ' ? space_count++ : 0);

    char *analyzed;
    ALLOCATE_MEM(analyzed, strlen(str) + space_count + 1, sizeof(char));
    for (int i = 0, k = 0; str[i]; i++, k++) {
        if (str[i] == ' ') {
            analyzed[k] = '\\';
            k++;
        }
        analyzed[k] = str[i];
    }

    return analyzed;
}