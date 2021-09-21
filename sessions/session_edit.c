#include <session_edit.h>
#include <session_parser.h>
#include <config_parser.h>
#include <malloc.h>
#include <string.h>
#include <include/codes.h>
#include <include/logger.h>

static char* check_current() {
    char* current;
    int result = get_current(&current);
    if(result != SUCCESS)
        logger(WARNING, "Failed to find current session with error code: ", result);

    return current;
}

static char* analyze_spaces_to_path(char* path) {
    int space_count = 0;
    // count spaces
    for (int i = 0; path[i];  i++, path[i] == ' ' ? space_count++ : 0);

    char* analyzed_path = (char*) calloc(strlen(path) + space_count + 1, sizeof(char));
    for(int i = 0, k = 0; path[i]; i++, k++) {
        if(path[i] == ' ') {
            analyzed_path[k] = '\\';
            k++;
        }
        analyzed_path[k] = path[i];
    }

    return path;
}

void session_command_exit(){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    int result = set_current("");
    if (result == SUCCESS)
        logger(INFO, "Exited current session.");
    else
        logger(ERROR, "Failed to exit current session with error code: ", result);
}

void session_command_undo(){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    int result = delete_last_line(current);
    if(result != SUCCESS)
        logger(ERROR, "Failed to undo command with error code: ", result);
}

void session_command_mkdir(char* dst, char* folder, char* permissions){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(6 + strlen(dst) + 1 + strlen(folder) + 1 + strlen(permissions) + 1, sizeof(char));
    strcat(cmd, "mkdir ");
    strcat(cmd, analyze_spaces_to_path(dst));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(folder));
    strcat(cmd, " ");
    strcat(cmd, permissions);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_mkfile(char* dst, char* file, char* permissions){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(7 + strlen(dst) + 1 + strlen(file) + 1 + strlen(permissions) + 1, sizeof(char));
    strcat(cmd, "mkfile ");
    strcat(cmd, analyze_spaces_to_path(dst));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(file));
    strcat(cmd, " ");
    strcat(cmd, permissions);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_copy(char* src, char* dst){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1, sizeof(char));
    strcat(cmd, "copy ");
    strcat(cmd, analyze_spaces_to_path(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(dst));

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_move(char* src, char* dst){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1, sizeof(char));
    strcat(cmd, "move ");
    strcat(cmd, analyze_spaces_to_path(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(dst));

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_rename(char* src, char* name){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(7 + strlen(src) + 1 + strlen(name) + 1, sizeof(char));
    strcat(cmd, "rename ");
    strcat(cmd, analyze_spaces_to_path(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(name));

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_edit(char* src, char* flag, char* content){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(flag) + 1 + strlen(content) + 1, sizeof(char));
    strcat(cmd, "edit ");
    strcat(cmd, analyze_spaces_to_path(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_spaces_to_path(content));
    strcat(cmd, " ");
    strcat(cmd, flag);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}

void session_command_permission(char* src, char* permissions, char* recursive){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    if(strcmp(recursive, "0") != 0 && strcmp(recursive, "1") != 0)
        return logger(ERROR, "Recursive must be 0 or 1");

    char* cmd = (char*) calloc(12 + strlen(src) + 1 + strlen(permissions) + 1 + strlen(recursive) + 1, sizeof(char));
    strcat(cmd, "permissions ");
    strcat(cmd, analyze_spaces_to_path(src));
    strcat(cmd, " ");
    strcat(cmd, permissions);
    strcat(cmd, " ");
    strcat(cmd, recursive);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        logger(ERROR, "Failed to add command with error code: ", result);

    free(cmd);
}