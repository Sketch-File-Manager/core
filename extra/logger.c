#include <stdio.h>
#include <stdarg.h>
#include "include/logger.h"

void logger(int priority, char* message, ...) {
    // TODO - priority to 0 -> INFO, 1 -> WARNING, 2 -> ERROR, 3 -> DEBUG
    printf("%d - ", priority);

    va_list args;
    va_start(args, message);
    char* msg = va_arg(args, char*);

    // TODO - If 'msg' is number then replace with error message. if msg is positive read errno.
    printf("%s", msg);

    va_end(args);

    putchar('\n');
}