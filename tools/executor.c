#include <string.h>
#include <stdio.h>
#include "executor.h"

int execute(char* command) {
    char* cmd = strtok(command, " ");

    printf("%s", cmd);

    return 0;
}