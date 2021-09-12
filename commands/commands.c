#include <codes.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <session_parser.h>
#include <dirent.h>
#include <fcntl.h>
#include <commands.h>
#include <file_handler.h>
#include <stdio.h>
#include <unistd.h>

typedef struct perm_queue {
    char   *p_path;
    int     p_is_dir;
} perm_queue;

static void read_contents_of(const char *path, perm_queue **queue, size_t queue_s) {
    DIR *dir = NULL;
    struct dirent *dir_contents = NULL;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL) {
        // Save the path.
        queue[queue_s - 1] = calloc(1, sizeof(perm_queue));
        queue[queue_s - 1]->p_path = calloc(strlen(path) + strlen(dir_contents->d_name) + 1, sizeof(char));
        strcpy(queue[queue_s - 1]->p_path, path);
        strcat(queue[queue_s - 1]->p_path, dir_contents->d_name);

        // Set True if the current content is folder.
        if (dir_contents->d_type != DT_REG)
            queue[queue_s - 1]->p_is_dir = TRUE;
        else
            queue[queue_s - 1]->p_is_dir = FALSE;

        // Increase the size of queue.
        queue = realloc(queue, ++queue_s);
    }
}

static inline int copy_content_of(char *src, char *dst) {

    // TODO extract the file name from the source file.
    // TODO add the extracted name in the dst_file path.
    int dst_fd = open(dst, O_CREAT, 700);
    if (dst_fd == -1) return FALSE;

    size_t src_file_s = strlen(src);

    if (write_file(dst, src, src_file_s) == -1) return FALSE;
    
    close(dst_fd);

    return TRUE;
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
    if (new_fd == -1) return FALSE;

    return SUCCESS;
}

int command_copy(char* src, char* dst_folder) {
    int src_fd = open(src, O_RDONLY);

    if (src_fd == -1) return FALSE;

    char *src_contents = NULL;

    if (read_file(src, &src_contents) == -1) return FALSE;

    copy_content_of(src_contents, dst_folder);

    close(src_fd);
    return SUCCESS;
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
        size_t queue_s = 1;
        perm_queue **queue = calloc(1, sizeof(perm_queue *));
        char *tmp;
        // Read the first contents.
        read_contents_of(src, queue, queue_s);

        while (queue != NULL) {
            int result = chmod(queue[queue_s - 1]->p_path, permissions);
            if(result == -1)
                return errno;

            // temporary save the current dir.
            if (queue[queue_s - 1]->p_is_dir == TRUE) {
                tmp = calloc(strlen(queue[queue_s - 1]->p_path) + 1, sizeof(char));
                strcpy(tmp, queue[queue_s - 1]->p_path);
            }

            // free the current dir.
            free(queue[queue_s - 1]->p_path);
            free(queue[queue_s - 1]);

            // decrease the size of the queue ( remove the last element ).
            queue = realloc(queue, --queue_s);

            if (queue[queue_s - 1]->p_is_dir == TRUE)
                read_contents_of(tmp, queue, queue_s);
        }
        free(queue);
        return SUCCESS;
    }

    int result = chmod(src, permissions);
    if(result == -1)
        return errno;

    return SUCCESS;
}