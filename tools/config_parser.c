#include <string.h>

#include <config_parser.h>
#include <file_handler.h>
#include <include/codes.h>
#include <include/functions.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int create_config_file() {
    char *absolute_path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(absolute_path, O_CREAT, 0700);
    int result = write_file(absolute_path, CURRENT_SESSION_ID, strlen(CURRENT_SESSION_ID));
    if (result != SUCCESS) {
        free(absolute_path);
        return result;
    }

    close(config_fd);
    free(absolute_path);

    return SUCCESS;
}

int get_current(char **current) {
    char *config = NULL;
    char *path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    int result = read_file(path, &config);
    if (result != SUCCESS) return result;

    // Find the location of current session in the file.
    char *location_of_interest = strstr(config, CURRENT_SESSION_ID);
    strtok(location_of_interest, " ");
    // get the current session.
    char *current_session = strtok(NULL, "\n");

    if (current_session == NULL) {
        *current = calloc(1, sizeof(char));
        strcpy(*current, "");
        return SUCCESS;
    }

    // Allocate enough space for the current session.
    *current = calloc(strlen(current_session) + 1, sizeof(char));
    // set the current session to current.
    strcpy(*current, current_session);

    free(config);
    free(path);
    return SUCCESS;
}

int set_current(const char *current) {
    // Calculate the size of the new current session.
    size_t new_current_session_s = strlen(current) + strlen(CURRENT_SESSION_ID);
    // Allocate enough memory for the new current session.
    char *new_current_session = calloc(new_current_session_s + 3, sizeof(char));
    // Set the new current session.
    strcpy(new_current_session, CURRENT_SESSION_ID);
    strcat(new_current_session, " ");
    strcat(new_current_session, current);
    strcat(new_current_session, "\n");

    char *absolute_path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    // Make the changes in the config file.
    int result = write_file(absolute_path, new_current_session, new_current_session_s + 2);
    free(new_current_session);
    free(absolute_path);

    return result;
}
