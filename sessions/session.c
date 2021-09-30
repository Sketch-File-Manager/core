#include <session.h>
#include <stdlib.h>
#include <string.h>
#include <session_parser.h>
#include <config_parser.h>
#include <stdio.h>
#include <executor.h>
#include <include/codes.h>
#include <include/functions.h>
#include <include/logger.h>

static int check_current() {
    char *current;
    int config_result = get_option(CURRENT_SESSION_ID, &current);
    if (config_result != SUCCESS)
        return FALSE;

    if (strcmp(current, "") == 0)
        return FALSE;

    return TRUE;
}

void session_start() {
    if (check_current() == TRUE)
        return logger(WARNING, "A session is already in use.", NULL);

    int name_len = 10;
    char *name = (char *) calloc(name_len + 9 + 1, sizeof(char));
    strcat(name, rand_string(10));

    if (endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    int result = create_file(name);
    if (result == SUCCESS)
        logger(INFO, "Session created successfully.\n", "New session id: ", name, NULL);
    else
        logger(ERROR, "Failed to create session with error code: ", result, NULL);

    free(name);
}

void session_end(char *id) {
    if (check_current() == TRUE)
        return logger(WARNING, "A session is already in use.");

    char *name = (char *) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if (endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    int result = delete_file(name);
    if (result == SUCCESS)
        logger(INFO, "Session ended.", NULL);
    else
        logger(ERROR, "Session failed to end with error code: ", result, NULL);

    free(name);
}

void session_use(char *id) {
    if (check_current() == TRUE)
        return logger(WARNING, "A session is already in use.", NULL);

    char *name = (char *) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if (endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    if (session_exists(name) == TRUE) {
        int result = set_option(CURRENT_SESSION_ID, name);
        if (result == SUCCESS)
            logger(INFO, "Current session is set to: ", name, NULL);
        else
            logger(ERROR, "Failed to use session with error code: ", result, NULL);
    } else logger(WARNING, "Session does not exist.", NULL);

    free(name);
}

void session_run(char *id) {
    if (check_current() == TRUE)
        return logger(WARNING, "A session is already in use.", NULL);

    char *name = (char *) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if (endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    char **lines;
    size_t n;
    int read_result = read_session(name, &lines, &n);

    if (read_result == SUCCESS) {
        // Check if it is executed
        if (strcmp(lines[0], "executed") != 0) {
            int flag = 0;

            // Add to the start as executed.
            int append_result = append_to_start(name, "executed");
            if (append_result != SUCCESS) {
                logger(ERROR, "Failed to execute session with error code: ", append_result, NULL);
                flag = 1;
            }

            if (flag != 1) {
                // Execute with system call.
                for (int i = 0; i < n; ++i) {
                    if (flag == 1) continue;

                    char *line = lines[i];
                    int exec_result = execute(line);

                    if (exec_result != SUCCESS) {
                        logger(ERROR, "Failed to execute a part of the session with error code: ", exec_result,
                               "\nException line: ", i, NULL);
                        flag = 1;
                    }
                }
            }
        } else logger(WARNING, "Session is already executed.", NULL);
    } else logger(ERROR, "Failed to read session with error code: ", read_result, NULL);

    free(name);
    for (int i = 0; i < n; ++i)
        free(lines[i]);
    free(lines);
}

void session_current() {
    char *current;
    int config_result = get_option(CURRENT_SESSION_ID, &current);
    if (config_result != SUCCESS)
        return logger(ERROR, "Cannot access current session.", NULL);

    if (strcmp(current, "") == 0)
        logger(INFO, "No session is selected.", NULL);
    else logger(INFO, current);

    free(current);
}

void session_show(char *id) {
    if (check_current() == TRUE)
        return logger(WARNING, "A session is already in use.", NULL);

    char *name = (char *) calloc(strlen(id) + 9 + 1, sizeof(char));
    strcat(name, id);

    if (endsWith(name, ".session") == FALSE)
        strcat(name, ".session");

    char **lines;
    size_t n;

    int result = read_session(name, &lines, &n);
    if (result != SUCCESS)
        logger(ERROR, "Failed to open session with error code: ", result, NULL);
    else {
        for (size_t i = 0; i < n; i++)
            printf("%s\n", lines[i]);
        putchar('\n');
    }

    free(name);
    for (int i = 0; i < n; ++i)
        free(lines[i]);
    free(lines);
}

void session_list() {
    char **lines;
    size_t n;

    int result = list_sessions(&lines, &n);
    if (result != SUCCESS)
        logger(ERROR, "Failed to list sessions with error code: ", result, NULL);

    for (size_t i = 0; i < n; i++)
        printf("[%d] %s\n", ((int) i) + 1, lines[i]);

    for (int i = 0; i < n; ++i)
        free(lines[i]);
    free(lines);
}