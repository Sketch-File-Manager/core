/* logger.c */
#include <stdio.h>
#include <stdarg.h>
#include "include/logger.h"

void logger(int priority, char *message, ...) {
    switch (priority) {
        case INFO:
            printf("\033[0;37mInfo - ");
            break;
        case WARNING:
            printf("\033[0;33mWarning - ");
            break;
        case ERROR:
            printf("\033[0;31mError - ");
            break;
        case DEBUG:
            printf("\033[0;36mDebug - ");
            break;
        default:
            printf("NoPriority - ");
    }

    va_list args;
    va_start(args, message);

    printf("%s", message);

    char *p;
    while (1) {
        p = va_arg(args, char *);

        if (p == NULL) break;
        printf("%s", p);
    }
    va_end(args);

    putchar('\n');
    printf("\033[0;37m");
}