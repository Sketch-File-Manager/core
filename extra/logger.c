#include <stdio.h>
#include <stdarg.h>
#include "include/logger.h"

void logger(int priority, char *message, ...) {
    // TODO - Add terminal colors
    switch (priority) {
        case INFO:
            printf("Info - ");
            // White
            break;
        case WARNING:
            printf("Warning - ");
            // Dark yellow
            break;
        case ERROR:
            printf("Error - ");
            // Dark red
            break;
        case DEBUG:
            printf("Debug - ");
            // Light blue
            break;
        default:
            printf("NoPriority - ");
    }

    va_list args;
    va_start(args, message);

    printf("%s", message);

    char *p = "";
    while (1) {
        p = va_arg(args, char *);

        if (p == NULL) break;
        printf("%s", p);
    }
    va_end(args);

    putchar('\n');
}