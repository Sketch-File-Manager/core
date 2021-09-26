#include <argument_parser.h>
#include <file_handler.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


void testing(char *ch, ...) {

    va_list argp;
    char *tmp = calloc(strlen(ch) + 1, sizeof(char));
    strcpy(tmp, ch);

    va_start(argp, ch);
    while (strcmp(tmp, "") != 0) {
        printf("%s\n", tmp);
        tmp = va_arg(argp, char*);
    }
    va_end(argp);

}



int main(int argc, char **argv) {
    char *config =  "config.conf";
    char *sketch =  "sketch/";
    char *session = "sessions/";

    check_requirements(config, sketch, session);
    int result = parse(argc, argv);
    return result;
}
