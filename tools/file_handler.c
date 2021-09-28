#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <file_handler.h>
#include <dirent.h>
#include <include/functions.h>
#include <include/codes.h>
#include <include/queue.h>
#include <errno.h>
#include <stdio.h>

static int get_file_fd(const char *file_path, int flag, size_t *file_len) {
    // allocate enough space for the full path.
    char *path = str_add(file_path, NULL);

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
    char *with_home = fix_path(path, TRUE);
    char *absolute_path = str_add(with_home, name, NULL);
    free(with_home);

    // TODO check for errors.
    mkdir(absolute_path, 0700);
    free(absolute_path);
}

void check_requirements(const char *config_file, const char *sketch_folder_name) {
    // Sketch folder
    create_dir(sketch_folder_name, SKETCH_LOCATION);

    // Config file
    char *home = get_home_path();
    char *absolute_path = str_add(home, CONFIG_LOCATION, config_file, NULL);
    free(home);

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(absolute_path, O_CREAT, 0700);
    char *first_write = "current_session:";

    close(config_fd);

    if (write_file(absolute_path, first_write, strlen(first_write))) {
        free(absolute_path);
        return;
    }

    free(absolute_path);

    // Session folder
    mkdir(SESSION_FOLDER_LOCATION, 0700);
}

int list_files_names(const char *path, char ***result_files, size_t *size) {
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

        files[index] = str_add(current_file, NULL);

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

int get_info_of(char *path, file_info ***files, size_t *size) {
    struct stat curr_element_stat;
    queue *c_queue = create_empty_queue();

    file_info **tmp_files = calloc(1, sizeof(file_info *));
    tmp_files[0] = calloc(1, sizeof(file_info));
    read_contents_of(path, c_queue);

    char **curr_element_name; // file or directory name.
    size_t curr_element_name_s;
    char *removed_item;

    int current_path = 0;
    while (c_queue->size != 0) {
        int result = stat((const char *) peek(c_queue), &curr_element_stat);
        if (result == -1) {
            // Free the current allocated space and return error.
            free(c_queue);
            while (c_queue->size != 0) {
                removed_item = pop(c_queue);
                free(removed_item);
            }
            for (int fr = 0; fr < current_path; fr++) free(tmp_files[fr]);
            free(tmp_files[current_path]);
            free(tmp_files);
            return errno;
        }

        curr_element_name = split_except((char *) peek(c_queue), '/', '\0', &curr_element_name_s);

        tmp_files[current_path]->f_name = str_add(curr_element_name[curr_element_name_s - 1], NULL);
        tmp_files[current_path]->f_user_id = curr_element_stat.st_uid;
        tmp_files[current_path]->f_group_id = curr_element_stat.st_gid;
        tmp_files[current_path]->f_permissions = curr_element_stat.st_mode;
        tmp_files[current_path]->f_last_access = curr_element_stat.st_atim;
        tmp_files[current_path]->f_last_modify = curr_element_stat.st_mtim;
        tmp_files[current_path]->f_last_access = curr_element_stat.st_ctim;
        tmp_files[current_path]->f_serial_number = curr_element_stat.st_ino;
        tmp_files[current_path]->f_link_count = curr_element_stat.st_nlink;
        tmp_files[current_path]->f_size = curr_element_stat.st_size;

        removed_item = pop(c_queue);
        if (is_dir((const char *) removed_item))
            read_contents_of((char *) peek(c_queue), c_queue);

        for (int fr = 0; fr < curr_element_name_s; fr++) free(curr_element_name[fr]);

        free(curr_element_name);
        free(removed_item);
        ++current_path;
        tmp_files = realloc(tmp_files, sizeof(file_info *) * (current_path + 1));
        tmp_files[current_path] = calloc(1, sizeof(file_info));
    }
    *size = current_path;
    *files = tmp_files;

    free(c_queue);
    return SUCCESS;
}