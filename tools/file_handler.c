#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>

#include <file_handler.h>

static int get_file_fd(char *file_path, int flag, size_t *file_len) {
    // allocate enough space for the full path.
    char *username = getlogin();
    size_t path_s = strlen(file_path) + strlen(username) + strlen("/home/") + strlen("/.local/share/sketch/");
    char *path = calloc(path_s + 1, sizeof(char));

    // form the full path.
    strcat(path, "/home/");
    strcat(path, username);
    strcat(path, "/.local/share/sketch/");
    strcat(path, file_path);

    // open the config file.
    int fd = open(path, flag);
    struct stat config_stat;

    if (file_len != NULL) {
        if (lstat(path, &config_stat) == -1) {
            return -1;
        }
        *file_len = config_stat.st_size;
    }

    free(path);
    return fd;
}

int read_file(char *file_path, char **config_content) {
    size_t file_len;
    // get the file descriptor of the config.
    int fd = get_file_fd(file_path, O_RDONLY, &file_len);
    char *buffer;

    // if something goes wrong.
    if (fd == -1) return -1;

    // allocate enough space for the content of the file.
    buffer = calloc(file_len, sizeof(char));
    // read the file.
    if (read(fd, buffer, file_len) == -1)
        return -1;

    // Give the results.
    *config_content = calloc(file_len + 1, sizeof(char));
    strncpy(*config_content, buffer, file_len);

    free(buffer);
    close(fd);

    return 0;
}

int write_file(char *file_path, char *changes, size_t changes_len) {
    int config_fd = get_file_fd(file_path, O_RDWR | O_TRUNC, NULL);

    // check if something goes wrong.
    if (config_fd == -1) return -1;
    // write the file.
    if (write(config_fd, changes, changes_len) == -1) return -1;

    // close the file.
    close(config_fd);
    return 0;
}

