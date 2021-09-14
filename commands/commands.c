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


static void read_contents_of(const char *path, queue *c_queue) {
    DIR *dir = NULL;
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

static inline int copy_file_content_of(char *src_file, char *dst_file) {

    // Open the source file.
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) return errno;

    char *src_contents = NULL;

    // Read the content of the source file.
    int read_result = read_file(src_file, &src_contents);
    if (read_result == -1) return read_result;


    // Create a new file at the destination.
    int dst_fd = open(dst_file, O_CREAT, 700);
    if (dst_fd == -1) return errno;

    size_t src_file_s = strlen(src_file);

    // Write the contents of the src to destination.
    int result = write_file(dst_file, src_file, src_file_s);
    if (result != SUCCESS) return result;

    close(src_fd);
    close(dst_fd);
    return SUCCESS;
}

static int copy_folder_content_of(char *src_folder, char *dst_folder) {
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
    int copy_result = copy_file_content_of(src, dst_folder);

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
            int result = chmod((const char *) c_queue->q_first_node, permissions);
            if(result == -1)
                return errno;

            // temporary save the current dir.
            // TODO check if the current element in the queue is directory.
            /*if (queue[queue_s - 1]->p_is_dir == TRUE) {
                tmp = calloc(strlen(queue[queue_s - 1]->p_path) + 1, sizeof(char));
                strcpy(tmp, queue[queue_s - 1]->p_path);
            }*/
            // TODO check the same.
            /*if (queue[queue_s - 1]->p_is_dir == TRUE)
                read_contents_of(tmp, queue, queue_s);*/

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