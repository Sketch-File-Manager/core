/* file_handler.c */
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include <files/file_handler.h>
#include <functions.h>
#include <queue.h>
#include <mem.h>

int read_file(const char *file_path, char **config_content) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) return FAILED;

    size_t file_size = get_file_size(file_path);

    char buffer[file_size];
    // Initialize buffer.
    memset(buffer, 0x0, file_size);

    // Get the contents of the file.
    if (read(fd, &buffer, file_size) == -1) return FAILED;

    // Copy the file to the destination.
    memcpy(*config_content, buffer, file_size);
    return SUCCESS;
}

int copy_with_byte_rate(const char *src, const char *dst, size_t rate) {
    // Get the permissions of the source file.
    mode_t file_permissions = get_permissions_of(src);

    // Open or create the destination file with append mode.
    int dst_fd = open(dst, O_RDONLY);
    // If the file already exists.
    if (dst_fd != -1) {
        close(dst_fd);
        return SUCCESS;
    }
    // Open the source file.
    int src_fd = open(src, O_RDONLY);
    dst_fd = open(dst, O_CREAT | O_APPEND | O_WRONLY, file_permissions);
    if (src_fd == -1 || dst_fd == -1) return errno;

    char buffer[rate];
    ssize_t result = read(src_fd, &buffer, rate);
    while (result && result != -1) {
        if (write(dst_fd, buffer, result) == -1) return errno;
        result = read(src_fd, buffer, rate);
    }

    if (result == -1) return errno;

    close(src_fd);
    close(dst_fd);
    return SUCCESS;
}


int write_file(const char *file_path, char *changes, size_t changes_len) {
    int config_fd = open(file_path, O_WRONLY | O_TRUNC);

    // check if something goes wrong.
    if (config_fd == -1) return errno;
    // write the file.
    if (write(config_fd, changes, changes_len) == -1) return errno;

    // close the file.
    close(config_fd);
    return SUCCESS;
}

// TODO - Make a function that automates the lines 79 - 92 for the two below functions.

int list_files_names(const char *path, char **result_files, size_t files)
{
    DIR *dir = opendir(path);
    struct dirent *dir_info;

    if (dir == NULL) return FAILED;

    int current_file = 0;
    while ((dir_info = readdir(dir)) != NULL &&
           current_file < files)
    {
        if (is_file(dir_info, dir_info->d_name, path))
            memcpy(result_files[current_file++],
                   dir_info->d_name, strlen(dir_info->d_name));
    }

    closedir(dir);
    return SUCCESS;
}

int get_files_info(const char *path, file_info *results, size_t files)
{
    // TODO - Make this function simple.
    return SUCCESS;
}