#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <file_handler.h>

static int get_config(char *file_path, int flag, size_t *config_len) {
    // allocate enough space for the full path.
    char *username = getlogin();
    char *config_path = calloc(strlen(file_path) + strlen(username) + strlen("/home/") + 2, sizeof(char));

    // form the full path.
    strcat(config_path, "/home/");
    strcat(config_path, username);
    strcat(config_path, "/");
    strcat(config_path, file_path);

    // open the config file.
    int fd = open(config_path, flag);
    struct stat config_stat;

    if (config_len != NULL) {
        if (lstat(config_path, &config_stat) == -1) {
            return -1;
        }
        *config_len = config_stat.st_size;
    }

    free(config_path);
    return fd;
}

int read_config(char *file_path, char **config_content) {
    size_t config_len;
    // get the file descriptor of the config.
    int config_fd = get_config(file_path, O_RDONLY, &config_len);
    char *buffer;

    // if something goes wrong.
    if (config_fd == -1) return -1;

    // allocate enough space for the content of the file.
    buffer = calloc(config_len, sizeof(char));
    // read the file.
    if (read(config_fd, buffer, config_len) == -1)
        return -1;

    // Give the results.
    *config_content = calloc(config_len + 1, sizeof(char));
    strcpy(*config_content, buffer);

    free(buffer);
    close(config_fd);

    return 0;
}

int write_config(char *file_path, char *changes, size_t changes_len) {
    int config_fd = get_config(file_path, O_RDWR | O_TRUNC, NULL);

    // check if something goes wrong.
    if (config_fd == -1) return -1;
    // write the file.
    if (write(config_fd, changes, changes_len) == -1) return -1;

    // close the file.
    close(config_fd);
    return 0;
}

