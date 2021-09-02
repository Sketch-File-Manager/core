#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <file_handler.h>

#define SKETCH_FOLDER "/.local/share/sketch/"

static int get_file_fd(char *file_path, int flag, size_t *file_len) {
    // allocate enough space for the full path.
    char *username = getlogin();
    size_t path_s = strlen(file_path) + strlen(username) + strlen("/home/") + strlen(SKETCH_FOLDER);
    char *path = calloc(path_s + 1, sizeof(char));

    // form the full path.
    strcat(path, "/home/");
    strcat(path, username);
    strcat(path, SKETCH_FOLDER);
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

static inline void create_dir(char *path) {
    char *username = getlogin();
    size_t sketch_path_s = strlen("/home/") + strlen(username) + strlen(path);
    char *sketch_path = calloc(sketch_path_s + 1, sizeof(char));

    strcpy(sketch_path, "/home/");
    strcat(sketch_path, username);
    strcat(sketch_path, path);

    mkdir(sketch_path, 0700);
    free(sketch_path);
}

static inline void check_sketch_folder(char *sketch_path) {
    create_dir(sketch_path);
}

static inline void check_config_file(char *path) {
    char *username = getlogin();
    size_t config_path_s = strlen("/home/") + strlen(username) + strlen(path);
    char *config_path = calloc(config_path_s + 1, sizeof(char));

    strcpy(config_path, "/home/");
    strcat(config_path, username);
    strcat(config_path, path);

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(config_path, O_CREAT, 0700);
    char *first_write = "current:\n";

    close(config_fd);
    if (write_file("config.conf", first_write, strlen(first_write))) return;

    free(config_path);

}

static inline void check_session_folder(char *session_path) {
    create_dir(session_path);
}

void check_requirements(char *config_path, char *sketch_path, char *sessions_path) {
    check_sketch_folder(sketch_path);
    check_config_file(config_path);
    check_session_folder(sessions_path);
}


