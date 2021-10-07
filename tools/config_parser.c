#include <string.h>

#include <config_parser.h>
#include <file_handler.h>
#include <include/codes.h>
#include <include/functions.h>
#include <fcntl.h>
#include <unistd.h>
#include <mem.h>


int create_config_file() {
    // TODO - Add the option for the byte rate in the config file with default value 516 bytes.
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

int get_option(const char *option, char **result) {
    char *config = NULL;
    char *path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    int code = read_file(path, &config);
    if (code != SUCCESS) return code;

    // Find the location of current session in the file.
    char *location_of_interest = strstr(config, option);
    strtok(location_of_interest, " ");
    // get the current session.
    char *current_session = strtok(NULL, "\n");

    if (current_session == NULL) {
        ALLOCATE_MEM(*result, 1, sizeof(char));
        strcpy(*result, "");
        return SUCCESS;
    }

    // Allocate enough space for the current session.
    ALLOCATE_MEM(*result, strlen(current_session) + 1, sizeof(char));
    // set the current session to current.
    strcpy(*result, current_session);

    free(config);
    free(path);

    return SUCCESS;
}

int set_option(const char *option, const char *value) {
    // TODO - Make set option.
    return SUCCESS;
}

int set_byte_rate(const char *value) {
    return set_option(BYTE_RATE, value);
}

/*
int set_current(const char *current) {
    // Calculate the size of the new current session.
    size_t new_current_session_s = strlen(current) + strlen(CURRENT_SESSION_ID);
    // Allocate enough memory for the new current session.
    char *new_current_session = str_add(CURRENT_SESSION_ID, " ", current, "\n", NULL);
    char *absolute_path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    // Make the changes in the config file.
    int result = write_file(absolute_path, new_current_session, new_current_session_s + 2);
    free(new_current_session);
    free(absolute_path);

    return result;
}
*/
