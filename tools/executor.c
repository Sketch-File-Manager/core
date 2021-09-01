#include <string.h>
#include <stdio.h>
#include <codes.h>
#include "executor.h"

int execute(char* command) {
    char* cmd = strtok(command, " ");

    printf("%s", command);
    printf("%s", cmd);

    if(strcmp(cmd, "mkdir") == 0){
        
    }
    else if(strcmp(cmd, "mkfile") == 0){

    }
    else if(strcmp(cmd, "copy") == 0){

    }
    else if(strcmp(cmd, "move") == 0){

    }
    else if(strcmp(cmd, "rename") == 0){

    }
    else if(strcmp(cmd, "edit") == 0){

    }


    return SUCCESS;
}