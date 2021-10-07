/* commands.c */
#include <include/codes.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <commands.h>
#include <file_handler.h>
#include <include/functions.h>
#include <include/queue.h>
#include <stdio.h>
#include <mem.h>

/** ================ COMMANDS ================ */

int command_mkdir(char *dst_folder, char *name, mode_t permissions) {
    char *folder_path;
    ALLOCATE_MEM(folder_path, strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(folder_path, dst_folder);
    strcpy(folder_path, name);

    int result = mkdir(folder_path, permissions);
    if (result == 0)
        return SUCCESS;

    return errno;
}

int command_mkfile(char *dst_folder, char *name, mode_t permissions) {
    char *destination;
    ALLOCATE_MEM(destination, strlen(dst_folder) + strlen(name) + 1, sizeof(char));
    strcpy(destination, dst_folder);
    strcat(destination, name);

    int new_fd = open(destination, O_CREAT, permissions);
    if (new_fd == -1) return errno;

    return SUCCESS;
}

static int copy_dir_contents(char *src, const char *dst) {
    queue *c_queue = create_empty_queue();
    read_contents_of(src, c_queue);
    char *send_to;

    mode_t element_perms;
    char *element_name;
    char *removed;

    char **element_split;
    size_t element_split_s = 0;

    char *root_folder;
    // Save the deeper sub folder of the src.
    element_split = split_except(src, '/', '\0', &element_split_s);
    ALLOCATE_MEM(root_folder, strlen(element_split[element_split_s - 2]) + 1, sizeof(char));
    strcpy(root_folder, element_split[element_split_s - 2]);
    FREE_ARRAY(element_split, element_split_s);
    element_split_s = 0;

    char *tmp;
    char *tmp_path;
    int flag = FALSE;

    int result = SUCCESS;
    while (c_queue->q_size != 0) {
        // Get the name of the current element.
        element_split = split_except((char *) peek(c_queue), '/', '\0', &element_split_s);

        ALLOCATE_MEM(element_name, strlen(element_split[element_split_s - 1]) + 1, sizeof(char));
        strcpy(element_name, element_split[element_split_s - 1]);

        // Check if the current element is either folder . or folder ..
        if (strcmp(element_name, ".") == 0 || strcmp(element_name, "..") == 0) {
            removed = pop(c_queue);
            free(removed);
            FREE_ARRAY(element_split, element_split_s);
            free(element_name);
            continue;
        }

        ALLOCATE_MEM(tmp, 1, sizeof(char));
        ALLOCATE_MEM(tmp_path, 2, sizeof(char));
        strcpy(tmp_path, "/");

        // Build the sub folder sequence for the current element.
        /* @element = file or directory.
           exp: if the current src is /home/username/testing and the
           current element is the /test/copy.txt
           and the destination is /home/username/test/
           then the bellow 18 lines build up the test/copy.txt
           witch is the sub path of the src. This is necessary to concatenate in the right of the
           destination path, in order to build the correct location for the specific element.
           end result: /home/username/test/test/copy.txt
                      |                   |    |        |
                       ------------------  ---   ------
                            |                |       |
                            v                v       v
                       destination      sub path   file
        */
        for (int curr_el = 0; curr_el < element_split_s; curr_el++) {
            REALLOCATE_MEM(tmp_path, (strlen(tmp_path) + strlen(element_split[curr_el]) + 2) * sizeof(char));
            strcat(tmp_path, element_split[curr_el]);
            strcat(tmp_path, "/");

            if (strcmp(root_folder, element_split[curr_el]) == 0 && flag != TRUE) {
                flag = TRUE;
                continue;
            }

            if (flag == TRUE) {
                REALLOCATE_MEM(tmp, (strlen(tmp) + strlen(element_split[curr_el]) + 1) * sizeof(char));
                strcat(tmp, element_split[curr_el]);
                if (is_dir(tmp_path) == TRUE) {
                    REALLOCATE_MEM(tmp, (strlen(tmp) + 2) * sizeof(char));
                    strcat(tmp, "/");
                }
            }
        }
        // Build the path where the current element must send.
        send_to = str_add(dst, tmp, NULL);
        // Remove the current element from the queue.
        removed = pop(c_queue);

        free(tmp);
        free(tmp_path);
        // Check if the current element is directory.
        if (is_dir(removed) == TRUE) {
            // Fix the path ( for DIR ).
            if (removed[strlen(removed) - 1] != '/')
                tmp = str_add(removed, "/", NULL);
            // Get the permission of the directory.
            element_perms = get_permissions_of(removed);
            // Make a directory with the same name under the new path.
            mkdir(send_to, element_perms);
            // Read all the contents of the directory.
            read_contents_of(tmp, c_queue);
            free(tmp);
        }
            // If the current element is file, then just copy it to the destination.
        else result = copy_with_byte_rate(removed, send_to, 516);

        if (result != SUCCESS) return result;

        flag = FALSE;
        FREE_ARRAY(element_split, element_split_s);
        free(removed);
        free(send_to);
        free(element_name);
    }
    free(c_queue);
    free(root_folder);

    return result;
}

static int copy_file(char *src, const char *dst_folder) {
    char *fix = fix_path(dst_folder, TRUE);
    size_t src_split_s = 0;
    char **src_split = split_except(src, '/', '\0', &src_split_s);
    // form the dst path.
    char *dst = str_add(fix, src_split[src_split_s - 1], NULL);
    if (copy_with_byte_rate(src, dst, 516) != SUCCESS) return errno;

    free(fix);
    FREE_ARRAY(src_split, src_split_s);
    free(src_split);
    free(dst);

    return SUCCESS;
}

int command_copy(char *src, char *dst_folder) {
    if (is_dir(src) == TRUE) return copy_dir_contents(src, dst_folder);
    else return copy_file(src, dst_folder); // TODO - Don't take the rate by literal.
}

int command_move(char *src, char *dst_folder) {
    char *src_fix = fix_path(src, FALSE);
    char *dst_fix = fix_path(dst_folder, TRUE);

    size_t src_split_s = 0;
    char **src_split = split_except(src_fix, '/', '\0', &src_split_s);

    char *dst_full = str_add(dst_folder, src_split[src_split_s - 1]);
    int result = rename(src_fix, dst_full);

    FREE_ARRAY(src_split, src_split_s);
    free(dst_full);
    free(dst_fix);
    free(src_fix);

    return result;
}

int command_rename(char *src, char *new_name) {
    size_t src_split_s = 0;
    char **src_split = split_except(src, '/', '\0', &src_split_s);
    char *path;
    ALLOCATE_MEM(path, strlen(src_split[1]) + 2, sizeof(char));
    strcpy(path, "/");
    strcat(path, src_split[1]);

    // Form the path without the file/folder name, example: /home/username/name -> /home/username/
    for (int split = 2; split < src_split_s - 1; split++) {
        REALLOCATE_MEM(path, strlen(path) + strlen(src_split[split]) + 2);
        strcat(path, "/");
        strcat(path, src_split[split]);
        free(src_split[split]);
    }
    free(src_split[src_split_s - 1]);
    free(src_split);

    // Now we have the path where the file should be sent ( in this case we send it in the exactly same path to rename it.)
    // Form the new path ( with the new name ).
    char *renamed = str_add(path, "/", new_name, NULL);
    free(path);

    // Rename the file.
    if (rename(src, renamed) == -1) {
        free(renamed);
        return errno;
    }

    free(renamed);

    return SUCCESS;
}

int command_edit(char *src, char *flag, char *content) {
    // TODO - edit
    return SUCCESS;
}

int command_permissions(char *src, mode_t permissions, unsigned int recursive) {
    if (recursive == 1) {
        queue *c_queue = create_empty_queue();
        char *tmp;
        // Read the first contents.
        read_contents_of(src, c_queue);

        while (c_queue->q_size != 0) {
            int result = chmod((const char *) peek(c_queue), permissions);
            if (result == -1)
                return errno;

            if (is_dir((const char *) peek(c_queue)) == TRUE) {
                ALLOCATE_MEM(tmp, strlen((const char *) peek(c_queue)) + 1, sizeof(char));
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
    char *f_directory = fix_path(directory, TRUE);
    int result = get_info_of(f_directory, &list, &list_s);

    if (result != SUCCESS)
        return result;

    printf("[\n");
    for (int i = 0; i < list_s; i++) {
        printf("  {\n");
        // General
        printf("    \"name\": \"%s\"\n", list[i]->f_name);
        printf("    \"location\": \"%s\"\n", f_directory);
        printf("    \"permissions\": \"%u\"\n", list[i]->f_permissions & 0777);
        printf("    \"q_size\": \"%ld\"\n", list[i]->f_size);
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

    free(list[list_s]);
    free(list);
    free(f_directory);

    return SUCCESS;
}