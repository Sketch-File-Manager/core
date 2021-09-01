#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>

#include <file_handler.h>

static int get_file_fd(char *file_path, int flag, size_t *file_len) {
    // allocate enough space for the full path.
    char *username = getlogin();
    char *path = calloc(strlen(file_path) + strlen(username) + strlen("/home/") + 2, sizeof(char));

    // form the full path.
    strcat(path, "/home/");
    strcat(path, username);
    strcat(path, "/");
    strcat(path, file_path);

    printf("%s\n", path);
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

    printf("%s\n", buffer);
    // Give the results.
    *config_content = calloc(file_len + 1, sizeof(char));
    strcpy(*config_content, buffer);

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

