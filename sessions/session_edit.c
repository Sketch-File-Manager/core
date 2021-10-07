/* session_edit.c */
#include <session_edit.h>
#include <session_parser.h>
#include <config_parser.h>
#include <malloc.h>
#include <string.h>
#include <include/codes.h>
#include <include/logger.h>
#include <include/functions.h>

static char *check_current() {
    char *current;
    int result = get_option(CURRENT_SESSION_ID, &current);
    if (result != SUCCESS)
        logger(WARNING, "Failed to find current session with error code: ", to_string(result), NULL);

    return current;
}

void session_command_exit() {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    int result = set_option(CURRENT_SESSION_ID,"");
    if (result == SUCCESS)
        logger(INFO, "Exited current session.", NULL);
    else
        logger(ERROR, "Failed to exit current session with error code: ", to_string(result), NULL);
}

void session_command_undo() {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    int result = delete_last_line(current);
    if (result != SUCCESS)
        logger(ERROR, "Failed to undo command with error code: ", to_string(result), NULL);
}

void session_command_mkdir(char *dst, char *folder, char *permissions) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_dst = analyze_string_spaces(dst);
    char *a_folder = analyze_string_spaces(folder);
    char *cmd = str_add("mkdir ", a_dst, " ", a_folder, " ", permissions, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_dst);
    free(a_folder);
    free(cmd);
}

void session_command_mkfile(char *dst, char *file, char *permissions) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_dst = analyze_string_spaces(dst);
    char *a_file = analyze_string_spaces(file);
    char *cmd = str_add("mkfile ", a_dst, " ", a_file, " ", permissions, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_dst);
    free(a_file);
    free(cmd);
}

void session_command_copy(char *src, char *dst) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_src = analyze_string_spaces(src);
    char *a_dst = analyze_string_spaces(dst);
    char *cmd = str_add("copy ", a_src, " ", a_dst, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_src);
    free(a_dst);
    free(cmd);
}

void session_command_move(char *src, char *dst) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_src = analyze_string_spaces(src);
    char *a_dst = analyze_string_spaces(dst);
    char *cmd = str_add("move ", a_src, " ", a_dst, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_src);
    free(a_dst);
    free(cmd);
}

void session_command_rename(char *src, char *name) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_src = analyze_string_spaces(src);
    char *cmd = str_add("rename ", a_src, " ", name, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_src);
    free(cmd);
}

void session_command_edit(char *src, char *flag, char *content) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    char *a_src = analyze_string_spaces(src);
    char *a_content = analyze_string_spaces(content);
    char *cmd = str_add("edit ", a_src, " ", flag, " ", a_content, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_src);
    free(a_content);
    free(cmd);
}

void session_command_permission(char *src, char *permissions, char *recursive) {
    char *current = check_current();
    if (strcmp(current, "") == 0) return;

    if (strcmp(recursive, "0") != 0 && strcmp(recursive, "1") != 0)
        return logger(ERROR, "Recursive must be 0 or 1", NULL);

    char *a_src = analyze_string_spaces(src);
    char *cmd = str_add("permissions ", a_src, " ", permissions, " ", recursive, NULL);

    int result = append_to_end(current, cmd);
    if (result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", to_string(result), NULL);

    free(a_src);
    free(cmd);
}