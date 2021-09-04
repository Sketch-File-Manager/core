#include <session_edit.h>
#include <session_parser.h>
#include <config_parser.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <codes.h>

static char* check_current() {
    char* current;
    int config_result = get_current(&current);
    if(config_result != SUCCESS)
        printf("%s %s %d\n", MSG_SESSION_CURRENT_NOT_FOUND, MSG_ERROR_CODE, config_result);

    return current;
}

static char* analyze_path_spaces(char* path) {
    int space_count = 0;
    for (int i = 0; path[i];  i++, path[i] == ' ' ? space_count : 0);

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

void command_exit(){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    int result = set_current("");
    if (result == SUCCESS)
        printf("%s\n", MSG_EXIT_CURRENT_SESSION);
    else
        printf("%s %S %d\n", MSG_CANNOT_EXIT_CURRENT_SESSION, MSG_ERROR_CODE, result);
}

void command_undo(){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    int edit_result = delete_last_line(current);
    if(edit_result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_UNDO_COMMAND, MSG_ERROR_CODE, edit_result);
}

void command_mkdir(char* dst, char* folder, char* permissions){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(6 + strlen(dst) + 1 + strlen(folder) + 1 + strlen(permissions) + 1, sizeof(char));
    strcat(cmd, "mkdir ");
    strcat(cmd, analyze_path_spaces(dst));
    strcat(cmd, " ");
    strcat(cmd, analyze_path_spaces(folder));
    strcat(cmd, " ");
    strcat(cmd, permissions);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_mkfile(char* dst, char* file, char* permissions){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(7 + strlen(dst) + 1 + strlen(file) + 1 + strlen(permissions) + 1, sizeof(char));
    strcat(cmd, "mkfile ");
    strcat(cmd, analyze_path_spaces(dst));
    strcat(cmd, " ");
    strcat(cmd, analyze_path_spaces(file));
    strcat(cmd, " ");
    strcat(cmd, permissions);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_copy(char* src, char* dst, char* permissions, char* recursive){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1 + strlen(permissions) + 1 + strlen(recursive) + 1, sizeof(char));
    strcat(cmd, "copy ");
    strcat(cmd, analyze_path_spaces(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_path_spaces(dst));
    strcat(cmd, " ");
    strcat(cmd, permissions);
    strcat(cmd, " ");
    strcat(cmd, recursive);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_move(char* src, char* dst, char* permissions, char* recursive){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1 + strlen(permissions) + 1 + strlen(recursive) + 1, sizeof(char));
    strcat(cmd, "move ");
    strcat(cmd, analyze_path_spaces(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_path_spaces(dst));
    strcat(cmd, " ");
    strcat(cmd, permissions);
    strcat(cmd, " ");
    strcat(cmd, recursive);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_rename(char* src, char* name){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(7 + strlen(src) + 1 + strlen(name) + 1, sizeof(char));
    strcat(cmd, "rename ");
    strcat(cmd, analyze_path_spaces(src));
    strcat(cmd, " ");
    strcat(cmd, analyze_path_spaces(name));

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_edit(char* src, char* flag, char* content){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(flag) + 1 + strlen(content) + 1, sizeof(char));
    strcat(cmd, "edit ");
    strcat(cmd, analyze_path_spaces(src));
    strcat(cmd, " ");
    strcat(cmd, flag);
    strcat(cmd, " ");
    strcat(cmd, content);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_permission(char* src, char* permissions, char* recursive){
    char* current = check_current();
    if(strcmp(current, "") == 0) return;

    if(strcmp(recursive, "0") != 0 && strcmp(recursive, "1") != 0) {
        printf("%s\n", MSG_INVALID_RECURSIVE_OPTION);
        return;
    }

    char* cmd = (char*) calloc(12 + strlen(src) + 1 + strlen(permissions) + 1 + strlen(recursive) + 1, sizeof(char));
    strcat(cmd, "permissions ");
    strcat(cmd, analyze_path_spaces(src));
    strcat(cmd, " ");
    strcat(cmd, permissions);
    strcat(cmd, " ");
    strcat(cmd, recursive);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d\n", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}