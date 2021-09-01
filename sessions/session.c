#include <session.h>
#include <stdlib.h>
#include <string.h>
#include <session_parser.h>
#include <config_parser.h>
#include <stdio.h>
#include <executor.h>
#include <codes.h>

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

static char* check_current() {
    char* current;
    int config_result = get_current(&current);
    if(config_result != SUCCESS)
        printf("Cannot get current session. Error code: %d", config_result);

    return current;
}

void session_start() {
    int name_len = 10;
    char* name = (char*) calloc(name_len + 9 + 1, sizeof(char));
    strcat(name, rand_string("", 10));
    strcat(name, ".session");

    int result = create_file(name);
    if(result != SUCCESS)
        printf("Failed to create session. Error code: %d", result);
    else
        printf("Created session: %s.", name);

    free(name);
}

void session_end(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, id);
    strcat(name, ".session");

    int result = delete_file(name);
    if(result != SUCCESS)
        printf("Failed to end session. Error code: %d", result);
    else
        printf("Ended session: %s.", name);

    free(name);
}

void session_use(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, id);
    strcat(name, ".session");

    if(session_exists(name) == TRUE) {
        int result = set_current(name);
        if (result != SUCCESS)
            printf("Failed to use session. Error code: %d", result);
        else
            printf("Using session: %s.", name);
    }
    else printf("Session: %s does not exist.", name);

    free(name);
}

void session_run(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, id);
    strcat(name, ".session");

    char** lines;
    size_t n;
    int read_result = read_session(name, &lines, &n);

    if(read_result == SUCCESS) {
        // Check if it is executed
        if(strcmp(lines[0], "executed") != 0) {
            int flag = 0;

            // Add to the start as executed.
            int append_result = append_to_start(name, "executed");
            if(append_result != SUCCESS) {
                printf("Cannot execute session. Error code: %d", read_result);
                flag = 1;
            }

            // Execute with system call.
            for (int i = 0; i < n; ++i) {
                if(flag == 1) continue;

                char* line = lines[i];
                int exec_result = execute(line);

                if(exec_result != SUCCESS) {
                    printf("Cannot execute session line %d: %s, aborting the rest of the session. Error code: %d", i, line, read_result);
                    flag = 1;
                }
            }
        }
        else printf("Session is already executed.");
    }
    else printf("Cannot read session. Error code: %d", read_result);

    free(name);
    for (int i = 0; i < n; ++i)
        free(lines[i]);
    free(lines);
}

void session_current() {
    char* current = check_current();
    if(current == NULL) return;
    if(strcmp(current, "") == 0)
        printf("No current sessions.");
    else
        printf("%s", current);
}

void session_show(char* id) {
    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(id, id);
    strcat(name, ".session");

    char** lines;
    size_t n;
    int result = read_session(name, &lines, &n);
    if(result != SUCCESS)
        printf("Cannot read session. Error code: %d", result);

    for(size_t i = 0; i < n; i++)
        printf("%s", lines[i]);
    putchar('\n');

    free(name);
}