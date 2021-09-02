#include <session.h>
#include <stdlib.h>
#include <string.h>
#include <session_parser.h>
#include <config_parser.h>
#include <stdio.h>
#include <executor.h>
#include <codes.h>
#include <time.h>
#include <functions.h>

static char *rand_string(size_t size){
    srand(time(NULL));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    char *str = (char*) calloc(size + 1, sizeof(char));
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

static int check_current() {
    char* current;
    int config_result = get_current(&current);
    if(config_result != SUCCESS)
        return FALSE;

    if(strcmp(current, "") == 0)
        return FALSE;

    return TRUE;
}

void session_start() {
    if(check_current() == TRUE) {
        printf("%s\n", SESSION_IN_USE);
        return;
    }

    int name_len = 10;
    char* name = (char*) calloc(name_len + 9 + 1, sizeof(char));
    strcat(name, rand_string(10));

    if(endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    int result = create_file(name);
    if (result == SUCCESS)
        printf("%s %s\n", MSG_SESSION_CREATED_SUCCESSFULLY, name);
    else
        printf("%s %s %d\n", MSG_SESSION_FAILED_TO_CREATE, MSG_ERROR_CODE, result);

    free(name);
}

void session_end(char* id) {
    if(check_current() == TRUE) {
        printf("%s\n", SESSION_IN_USE);
        return;
    }

    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if(endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    int result = delete_file(name);
    if (result == SUCCESS)
        printf("%s %s\n", MSG_SESSION_ENDED, name);
    else
        printf("%s %s %s %d\n", MSG_SESSION_FAILED_TO_END, name, MSG_ERROR_CODE, result);

    free(name);
}

void session_use(char* id) {
    if(check_current() == TRUE) {
        printf("%s\n", SESSION_IN_USE);
        return;
    }

    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if(endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    if(session_exists(name) == TRUE) {
        int result = set_current(name);
        if (result == SUCCESS)
            printf("%s %s\n", MSG_SESSION_USE_SUCCESSFUL, name);
        else
            printf("%s %s %s %d\n", MSG_SESSION_FAILED_TO_USE, name, MSG_ERROR_CODE, result);
    }
    else printf("%s %s\n", MSG_SESSION_DOES_NOT_EXIST, name);

    free(name);
}

void session_run(char* id) {
    if(check_current() == TRUE) {
        printf("%s\n", SESSION_IN_USE);
        return;
    }

    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if(endsWith(name, ".session") == FALSE)
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
                printf("%s %s %d\n", MSG_SESSION_CANNOT_EXECUTE, MSG_ERROR_CODE, read_result);
                flag = 1;
            }

            // Execute with system call.
            for (int i = 0; i < n; ++i) {
                if(flag == 1) continue;

                char* line = lines[i];
                int exec_result = execute(line);

                if(exec_result != SUCCESS) {
                    printf("%s Line %d: %s, %s. %s %d\n", MSG_SESSION_CANNOT_EXECUTE, i, line, MSG_SESSION_ABORT_REST, MSG_ERROR_CODE, read_result);
                    flag = 1;
                }
            }
        }
        else printf("%s\n", MSG_SESSION_ALREADY_EXECUTED);
    }
    else printf("%s %s %d\n", MSG_SESSION_CANNOT_OPEN, MSG_ERROR_CODE, read_result);

    free(name);
    for (int i = 0; i < n; ++i)
        free(lines[i]);
    free(lines);
}

void session_current() {
    char* current;
    int config_result = get_current(&current);
    if(config_result != SUCCESS) {
        printf("%s %s %d\n", MSG_SESSION_CURRENT_NOT_FOUND, MSG_ERROR_CODE, config_result);
        return;
    }

    if(strcmp(current, "") == 0)
        printf("%s\n", MSG_NO_CURRENT_SESSION);
    else
        printf("%s\n", current);
}

void session_show(char* id) {
    if(check_current() == TRUE) {
        printf("%s\n", SESSION_IN_USE);
        return;
    }

    char* name = (char*) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if(endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    char** lines;
    size_t n;

    int result = read_session(name, &lines, &n);
    if(result != SUCCESS)
        printf("Cannot read session. Error code: %d\n", result);

    for(size_t i = 0; i < n; i++)
        printf("%s\n", lines[i]);
    putchar('\n');

    free(name);
}

void session_list() {
    char** lines;
    size_t n;

    int result = list_sessions(&lines, &n);
    if(result != SUCCESS)
        printf("Cannot read session. Error code: %d\n", result);

    for(size_t i = 0; i < n; i++)
        printf("%s\n", lines[i]);
    putchar('\n');
}