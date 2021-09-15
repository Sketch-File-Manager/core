#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <file_handler.h>
#include <dirent.h>
#include <include/functions.h>
#include <include/codes.h>
#include <errno.h>

#define SKETCH_LOCATION     "/.local/share/"
#define CONFIG_LOCATION     "/.local/share/sketch/"
#define SESSION_LOCATION    "/.local/share/sketch/"


static int get_file_fd(const char *file_path, int flag, size_t *file_len) {
    // allocate enough space for the full path.
    size_t path_s = strlen(file_path);
    char *path = calloc(path_s + 1, sizeof(char));
    strcpy(path, file_path);

    // open the config file.
    int fd = open(path, flag);
    struct stat config_stat;

    if (file_len != NULL) {
        int result = lstat(path, &config_stat);
        if (result != 0)
            return -1;

        *file_len = config_stat.st_size;
    }

    free(path);
    return fd;
}

int read_file(const char *file_path, char **config_content) {
    size_t file_len;
    // get the file descriptor of the config.
    int fd = get_file_fd(file_path, O_RDONLY, &file_len);
    char *buffer;

    // if something goes wrong.
    if (fd == -1) return errno;

    // allocate enough space for the content of the file.
    buffer = calloc(file_len, sizeof(char));
    // read the file.
    if (read(fd, buffer, file_len) == -1)
        return errno;

    // Give the results.
    *config_content = calloc(file_len + 1, sizeof(char));
    strncpy(*config_content, buffer, file_len);

    free(buffer);
    close(fd);

    return SUCCESS;
}

int write_file(const char *file_path, char *changes, size_t changes_len) {
    int config_fd = get_file_fd(file_path, O_RDWR | O_TRUNC, NULL);

    // check if something goes wrong.
    if (config_fd == -1) return errno;
    // write the file.
    if (write(config_fd, changes, changes_len) == -1) return errno;

    // close the file.
    close(config_fd);
    return SUCCESS;
}

static inline void create_dir(const char *name, const char *path) {
    char *absolute_path = merge_home_relative_filename(name, path);

    // TODO check for errors.
    mkdir(absolute_path, 0700);
    free(absolute_path);
}

static inline void check_sketch_folder(const char *sketch_folder_name) {
    create_dir(sketch_folder_name, SKETCH_LOCATION);
}

static inline void check_config_file(const char *file_name) {
    char *absolute_path = merge_home_relative_filename(file_name, CONFIG_LOCATION);

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(absolute_path, O_CREAT, 0700);
    char *first_write = "current_session:";

    close(config_fd);
    if (write_file(absolute_path, first_write, strlen(first_write))) return;

    free(absolute_path);
}

static inline void check_session_folder(const char *session_folder_name) {
    create_dir(session_folder_name, SESSION_LOCATION);
}

void check_requirements(const char *config_file, const char *sketch_folder_name, const char *sessions_folder_name) {
    check_sketch_folder(sketch_folder_name);
    check_config_file(config_file);
    check_session_folder(sessions_folder_name);
}

int list_files(const char *path, char ***result_files, size_t *size) {
    DIR *dir = opendir(path);
    struct dirent *dir_info;

    if (dir == NULL) return errno;

    size_t files_s = 1;
    char **files = calloc(1, sizeof(char *));
    char *current_file = NULL;
    int index = 0;

    while ((dir_info = readdir(dir)) != NULL) {
        if (strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0) continue;

        current_file = dir_info->d_name;

        files[index] = calloc(strlen(current_file) + 1, sizeof(char ));
        strcpy(files[index], current_file);

        ++files_s;
        files = realloc(files, sizeof(char *) * files_s);
        index++;
    }
    --files_s;
    closedir(dir);
    *size = files_s;
    *result_files = files;

    return SUCCESS;
}