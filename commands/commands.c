#include <include/codes.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <session_parser.h>
#include <dirent.h>
#include <fcntl.h>
#include <commands.h>
#include <file_handler.h>
#include <unistd.h>
#include <include/functions.h>
#include <include/queue.h>
#include <stdio.h>


static int copy_file_content_of(char *src_file, char *dst_file) {
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

/** ================ COMMANDS ================ */

int command_mkdir(char *dst_folder, char *name, __mode_t permissions) {
    char *folder_path = calloc(strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(folder_path, dst_folder);
    strcpy(folder_path, name);

    int result = mkdir(folder_path, permissions);
    if (result == 0)
        return SUCCESS;

    return errno;
}

int command_mkfile(char *dst_folder, char *name, __mode_t permissions) {
    char *destination = calloc(strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(destination, dst_folder);
    strcat(destination, name);

    int new_fd = open(destination, O_CREAT, permissions);
    if (new_fd == -1) return errno;

    return SUCCESS;
}

int command_copy(char *src, char *dst_folder) {
    queue *c_queue = create_empty_queue();
    read_contents_of(src, c_queue);
    char *send_to;

    __mode_t current_path_perms;
    size_t current_path_split_s = 0;
    char **current_path_split = NULL;

    int result = SUCCESS;
    while (c_queue->size != 0) {
        current_path_split = split_except((char *) peek(c_queue), '/', '\0', &current_path_split_s);
        // Get the permissions of the current path.
        current_path_perms = get_permissions_of((char *) peek(c_queue));
        // Make the path to the new made directory or file.
        // File or directory name is located in the current_path_split[current_path_split_s - 1] based on split_except.
        send_to = calloc(strlen(dst_folder) + strlen(current_path_split[current_path_split_s - 1]) + 1, sizeof(char));
        // If the element that we are looking is directory. Then make a directory with the same name, under the new location.
        if (is_dir((const char *) pop(c_queue)) == TRUE) {
            // Create the directory. ( destination path ).
            mkdir(send_to, current_path_perms);
            // Search for more files in the new directory. ( source path ).
            read_contents_of((char *) peek(c_queue), c_queue);
        } else result = copy_file_content_of((char *) peek(c_queue), send_to);


        free(send_to);
        // Free the split_except.
        for (int fr = 0; fr < current_path_split_s; fr++) {
            free(current_path_split[fr]);
            current_path_split_s = 0;
        }

        if (result != SUCCESS) {
            free(c_queue);
            return result;
        }
    }
    free(c_queue);

    return SUCCESS;
}

int command_move(char *src, char *dst_folder) {
    command_copy(src, dst_folder);

    if (is_dir(src) == TRUE) {
        if (rmdir(src) == -1) return -1;
    } else {
        delete_file(src);
    }

    return SUCCESS;
}

int command_rename(char *src, char *new_name) {
    // TODO - rename
    return SUCCESS;
}

int command_edit(char *src, char *content, char *flag) {
    // TODO - edit
    return SUCCESS;
}

int command_permissions(char *src, __mode_t permissions, unsigned int recursive) {
    if (recursive == 1) {
        queue *c_queue = create_empty_queue();
        char *tmp;
        // Read the first contents.
        read_contents_of(src, c_queue);

        while (c_queue->size != 0) {
            int result = chmod((const char *) peek(c_queue), permissions);
            if (result == -1)
                return errno;

            if (is_dir((const char *) peek(c_queue)) == TRUE) {
                tmp = calloc(strlen((const char *) peek(c_queue)) + 1, sizeof(char));
                strcpy(tmp, (const char *) peek(c_queue));
                read_contents_of(tmp, c_queue);
            }

            pop(c_queue);
        }
        free(c_queue);
        return SUCCESS;
    }

    int result = chmod(src, permissions);
    if (result == -1)
        return errno;

    return SUCCESS;
}

int command_ls(char *directory) {
    file_info **list;
    size_t list_s = 0;
    char *fix = fix_path(directory, TRUE);
    int result = get_info_of(fix, &list, &list_s);
    if (result != SUCCESS)
        return result;

    printf("[\n");
    for (int i = 0; i < list_s; i++) {
        printf("  {\n");
        // General
        printf("    \"name\": \"%s\"\n", list[i]->f_name);
        printf("    \"location\": \"%s\"\n", fix_path(directory, TRUE));
        printf("    \"permissions\": \"%u\"\n", list[i]->f_permissions&0777);
        printf("    \"size\": \"%ld\"\n", list[i]->f_size);
        // Owners' ids
        printf("    \"group_id\": \"%u\"\n", list[i]->f_group_id);
        printf("    \"user_id\": \"%u\"\n", list[i]->f_user_id);
        // Timespec
        printf("    \"last_access\": \"%ld.%.9ld\"\n", list[i]->f_last_access.tv_sec, list[i]->f_last_access.tv_nsec);
        printf("    \"last_modify\": \"%ld.%.9ld\"\n", list[i]->f_last_modify.tv_sec, list[i]->f_last_modify.tv_nsec);
        printf("    \"last_status_change\": \"%ld.%.9ld\"\n", list[i]->f_status_change.tv_sec,
               list[i]->f_status_change.tv_nsec);
        // Other
        printf("    \"serial_number\": \"%lu\"\n", list[i]->f_serial_number);
        printf("    \"f_link_count\": \"%lu\"\n", list[i]->f_link_count);

        printf("  }\n");
    }
    printf("]\n");

    for (int element = 0; element < list_s; element++) {
        free(list[element]->f_name);
        free(list[element]);
    }

    free(list);
    free(fix);

    return SUCCESS;
}