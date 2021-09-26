#include <string.h>
#include <stdio.h>

#include <config_parser.h>
#include <file_handler.h>
#include <include/codes.h>
#include <include/functions.h>

#define CONFIG_FILE         "config.conf"
#define CONFIG_LOCATION     ".local/share/sketch/"
#define CURRENT_SESSION_ID  "current_session:"

int get_current(char** current) {
    char *config = NULL;
    char* home = get_home_path();
    char* path = str_add(home, CONFIG_LOCATION, CONFIG_FILE);
    free(home);

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

int set_current(const char* current) {
    // Calculate the size of the new current session.
    size_t new_current_session_s = strlen(current) + strlen(CURRENT_SESSION_ID);
    // Allocate enough memory for the new current session.
    char *new_current_session = calloc(new_current_session_s + 3, sizeof(char));
    // Set the new current session.
    strcpy(new_current_session, CURRENT_SESSION_ID);
    strcat(new_current_session, " ");
    strcat(new_current_session, current);
    strcat(new_current_session, "\n");

    char* home = get_home_path();
    char *absolute_path = str_add(home, CONFIG_LOCATION, CONFIG_FILE);
    free(home);

    // Make the changes in the config file.
    int result = write_file(absolute_path, new_current_session, new_current_session_s + 2);
    free(new_current_session);
    free(absolute_path);

    return result;
}
