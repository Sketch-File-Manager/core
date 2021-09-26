#include <stdio.h>
#include <stdarg.h>
#include "include/logger.h"

void logger(int priority, char* message, ...) {
    printf("%d - ", priority);

    va_list args;
    va_start(args, message);
    char* msg = va_arg(args, char*);
    printf("%s", msg);
    va_end(args);

    putchar('\n');
}