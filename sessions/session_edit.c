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
        printf("%s %s %d", MSG_SESSION_CURRENT_NOT_FOUND, MSG_ERROR_CODE, config_result);

    return current;
}

void command_exit(){
    char* current = check_current();
    if(current == NULL) return;

    int result = set_current("");
    if (result == SUCCESS)
        printf("%s", MSG_EXIT_CURRENT_SESSION);
    else
        printf("%s %S %d", MSG_CANNOT_EXIT_CURRENT_SESSION, MSG_ERROR_CODE, result);
}

void command_undo(){
    char* current = check_current();
    if(current == NULL) return;

    int edit_result = delete_last_line(current);
    if(edit_result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_UNDO_COMMAND, MSG_ERROR_CODE, edit_result);
}

void command_mkdir(char* dst, char* folder){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(6 + strlen(dst) + 1 + strlen(folder) + 1, sizeof(char));
    strcat(cmd, "mkdir ");
    strcat(cmd, dst);
    strcat(cmd, " ");
    strcat(cmd, folder);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_mkfile(char* dst, char* file){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(7 + strlen(dst) + 1 + strlen(file) + 1, sizeof(char));
    strcat(cmd, "mkfile ");
    strcat(cmd, dst);
    strcat(cmd, " ");
    strcat(cmd, file);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_copy(char* src, char* dst){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1, sizeof(char));
    strcat(cmd, "copy ");
    strcat(cmd, src);
    strcat(cmd, " ");
    strcat(cmd, dst);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_move(char* src, char* dst){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(dst) + 1, sizeof(char));
    strcat(cmd, "move ");
    strcat(cmd, src);
    strcat(cmd, " ");
    strcat(cmd, dst);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_rename(char* src, char* name){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(7 + strlen(src) + 1 + strlen(name) + 1, sizeof(char));
    strcat(cmd, "rename ");
    strcat(cmd, src);
    strcat(cmd, " ");
    strcat(cmd, name);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}

void command_edit(char* src, char* content, char* flag){
    char* current = check_current();
    if(current == NULL) return;

    char* cmd = (char*) calloc(5 + strlen(src) + 1 + strlen(flag) + 1 + strlen(content) + 1, sizeof(char));
    strcat(cmd, "edit ");
    strcat(cmd, src);
    strcat(cmd, " ");
    strcat(cmd, flag);
    strcat(cmd, " ");
    strcat(cmd, content);

    int result = append_to_end(current, cmd);
    if(result != SUCCESS)
        printf("%s %s %d", MSG_CANNOT_ADD_COMMAND, MSG_ERROR_CODE, result);

    free(cmd);
}