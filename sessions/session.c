#include <session.h>
#include <stdlib.h>
#include <string.h>
#include <session_parser.h>
#include <config_parser.h>
#include <stdio.h>

static char *rand_string(char *str, size_t size){
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* check_current() {
    char* current;
    int config_result = get_current(&current);
    if(config_result != -1)
        printf("Cannot get current session. Error code: %d", config_result);

    return current;
}

void session_start() {
    int name_len = 10;
    char* name = (char*) calloc(name_len + 9 + 1, sizeof(char));
    strcat(name, rand_string("", 10));
    strcat(name, ".session");

    create_file(name);
}

void session_end(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, rand_string("", 10));
    strcat(name, ".session");

    delete_file(name);
}

void session_use(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, rand_string("", 10));
    strcat(name, ".session");

    set_current(name);
}

void session_run(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, rand_string("", 10));
    strcat(name, ".session");
    // TODO - Execute with system call.
    //        Add to the start as executed.
}

void session_current() {
    char* current = check_current();
    if(current == NULL)
        printf("No current sessions.");
    else
        printf("%s", current);
}

void session_show(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, rand_string("", 10));
    strcat(name, ".session");

    char** lines;
    int result = read(name, &lines);
    if(result != -1) {
        printf("Cannot read session. Error code: %d", result);
        return;
    }

    for(size_t i = 0; i < sizeof(lines)/sizeof(lines[0]); i++)
        printf("%s", lines[i]);
    putchar('\n');
}