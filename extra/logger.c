#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

void log(int priority, char* message, ...) {
    printf("%d - ", priority);

    va_list args;
    va_start(args, message);
    printf("%s", va_arg(args, char*));
    va_end(args);

    putchar('\n');
}