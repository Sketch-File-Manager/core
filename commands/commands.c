#include <codes.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <session_parser.h>
#include <dirent.h>
#include <fcntl.h>
#include <commands.h>
#include <file_handler.h>
#include <unistd.h>
#include <functions.h>
#include <queue.h>
#include <stdio.h>


static void read_contents_of(const char *path, queue *c_queue) {
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

static inline int is_directory(const char *path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return -1;

    // do an and statement with bits of st_mode and bits of S_IFDIR.
    // If is the same the result is ok, otherwise the result is zero.
    if (path_stat.st_mode & S_IFDIR)
        return TRUE;

    return FALSE;
}

static inline __mode_t get_permissions_of(const char *path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return -1;

    return path_stat.st_mode;
}

static inline int copy_file_content_of(char *src_file, char *dst_file) {

    // Open the source file.
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) return errno;

    char *src_contents = NULL;

    // Read the content of the source file.
    int read_result = read_file(src_file, &src_contents);
    if (read_result == -1) return read_result;

    __mode_t file_permissions = get_permissions_of(src_file);
    // Create a new file at the destination.
    int dst_fd = open(dst_file, O_CREAT, file_permissions);
    if (dst_fd == -1) return errno;

    size_t src_file_s = strlen(src_file);

    // Write the contents of the src to destination.
    int result = write_file(dst_file, src_file, src_file_s);
    if (result != SUCCESS) return result;

    close(src_fd);
    close(dst_fd);
    return SUCCESS;
}

static int copy_content_of(char *src_folder, char *dst_folder) {
    queue *queue = create_empty_queue();
    read_contents_of(src_folder, queue);
    char *send_to;

    __mode_t current_path_perms;
    size_t current_path_split_s = 0;
    char **current_path_split = NULL;

    while (queue->size != 0) {
        current_path_split = split_with_exception((char *) queue->q_first_node, '/', NULL, &current_path_split_s);
        // Get the permissions of the current path.
        current_path_perms = get_permissions_of((char *) queue->q_first_node);
        // Make the path to the new made directory or file.
        // File or directory name is located in the current_path_split[current_path_split_s - 1] based on split.
        send_to = calloc(strlen(dst_folder) + strlen(current_path_split[current_path_split_s - 1]) + 1, sizeof(char));
        // If the element that we are looking is directory. Then make a directory with the same name, under the new location.
        if (is_directory((const char *) queue->q_first_node) == TRUE) {
            // Create the directory. ( destination path ).
            mkdir(send_to, current_path_perms);
            // Search for more files in the new directory. ( source path ).
            read_contents_of((char *) queue->q_first_node, queue);
        }
        else copy_file_content_of((char *) queue->q_first_node, send_to);

        free(send_to);
        // Free the split.
        for (int fr = 0; fr < current_path_split_s; fr++) {
            free(current_path_split[fr]);
            current_path_split_s = 0;
        }

        if (pop(queue) == -1) return 1;
    }
    
    return SUCCESS;
}

/** ================ COMMANDS ================ */

int command_mkdir(char* dst_folder, char* name, __mode_t permissions) {
    char* folder_path = calloc(strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(folder_path, dst_folder);
    strcpy(folder_path, name);

    int result = mkdir(folder_path, permissions);
    if(result == 0)
        return SUCCESS;

    return errno;
}

int command_mkfile(char* dst_folder, char* name, __mode_t permissions) {
    char *destination = calloc(strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(destination, dst_folder);
    strcat(destination, name);

    int new_fd = open(destination, O_CREAT, permissions);
    if (new_fd == -1) return errno;

    return SUCCESS;
}

int command_copy(char* src, char* dst_folder) {
    int copy_result = copy_content_of(src, dst_folder);

    return copy_result;
}

int command_move(char* src, char* dst_folder) {
    return SUCCESS;
}

int command_rename(char* src, char* new_name) {
    return SUCCESS;
}

int command_edit(char* src, char* content, char* flag) {
    return SUCCESS;
}

int command_permissions(char* src, __mode_t permissions, unsigned int recursive) {
    if(recursive == 1) {
        queue *c_queue = create_empty_queue();
        char *tmp;
        // Read the first contents.
        read_contents_of(src, c_queue);

        while (c_queue->size != 0) {
            int result = chmod((const char *) c_queue->q_first_node->q_item, permissions);
            if(result == -1)
                return errno;

            if (is_directory((const char *) c_queue->q_first_node) == TRUE) {
                tmp = calloc(strlen((const char *) c_queue->q_first_node->q_item) + 1, sizeof(char));
                strcpy(tmp, (const char *) c_queue->q_first_node->q_item);
                read_contents_of(tmp, c_queue);
            }

            pop(c_queue);
        }
        free(c_queue);
        return SUCCESS;
    }

    int result = chmod(src, permissions);
    if(result == -1)
        return errno;

    return SUCCESS;
}

int command_ls(char* directory) {
    char **list = NULL;
    size_t list_s = 0;

    int list_result = list_files(fix_path(directory, FALSE), &list, &list_s);
    if(list_result != SUCCESS) return list_result;

    for (int i = 0; i < list_s; i++)
        printf("[%d] %s\n", (int)i, list[i]);

    return SUCCESS;
}