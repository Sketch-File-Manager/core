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
    memset(buffer, 0, file_size);

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

int list_files_names(const char *path, char ***result_files, size_t files)
{
    DIR *dir = opendir(path);
    struct dirent *dir_info;
    struct stat    file_stat;

    if (dir == NULL || lstat(path, &file_stat) == -1) return FAILED;

    int current_file = 0;
    while ((dir_info = readdir(dir)) != NULL &&
           current_file < files)
    {
        if (is_file(dir_info, &file_stat))
            memcpy(*result_files[current_file],
                   dir_info->d_name, strlen(dir_info->d_name));
    }

    closedir(dir);
    return SUCCESS;
}

int get_info_of(char *path, file_info ***files, size_t *size) {
    /*struct stat curr_element_stat;
    queue *c_queue = create_empty_queue();

    file_info **tmp_files;
    ALLOCATE_MEM(tmp_files, 1, sizeof(file_info *));
    ALLOCATE_MEM(tmp_files[0], 1, sizeof(file_info));
    read_contents_of(path, c_queue);

    char **curr_element_name; // file or directory name.
    size_t curr_element_name_s;
    char *removed_item;

    int result;
    int current_path = 0;
    while (c_queue->q_size != 0) {
        curr_element_name = split_except((char *) peek(c_queue), '/', '\0', &curr_element_name_s);

        if (strcmp(curr_element_name[curr_element_name_s - 1], "..") == 0     ||
            strcmp(curr_element_name[curr_element_name_s - 1], ".")  == 0     ||
            strstr(curr_element_name[curr_element_name_s - 1], "..") != NULL  ||
            curr_element_name[curr_element_name_s - 1][strlen(curr_element_name[curr_element_name_s - 1]) - 1] == '.') {

            free(pop(c_queue));
            FREE_ARRAY(curr_element_name, curr_element_name_s);
            continue;
        }
        result = stat((const char *) peek(c_queue), &curr_element_stat);
        if (result == -1) {
            FREE_ARRAY(curr_element_name, curr_element_name_s);
            free(pop(c_queue));
            continue;
        }

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
        if (is_dir((const char *) removed_item) == TRUE)
            read_contents_of((char *) removed_item, c_queue);

        FREE_ARRAY(curr_element_name, curr_element_name_s);
        free(removed_item);
        ++current_path;
        REALLOCATE_MEM(tmp_files, sizeof(file_info *) * (current_path + 1));
        ALLOCATE_MEM(tmp_files[current_path], 1, sizeof(file_info));
    }
    *size = current_path;
    *files = tmp_files;

    free(c_queue);*/
    return SUCCESS;
}