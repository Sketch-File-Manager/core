/* config_parser.c */
#include <string.h>

#include <parsers/config_parser.h>
#include <files/file_handler.h>
#include "../general functions/include/functions.h"
#include <fcntl.h>
#include <unistd.h>
#include <mem.h>


int create_config_file() {
    char *absolute_path = fix_path(CONFIG_FILE_LOCATION, FALSE);
    char *config_file = str_add(CURRENT_SESSION_ID, ":\n",
                                "byte_rate: 516", "\n",
                                NULL);

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(absolute_path, O_CREAT, 0700);
    int result = write_file(absolute_path, config_file, strlen(config_file));
    if (result != SUCCESS) {
        free(absolute_path);
        return result;
    }

    close(config_fd);
    free(absolute_path);
    free(config_file);

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
    char *config = NULL;
    size_t config_s = 0;
    char *path = fix_path(CONFIG_FILE_LOCATION, FALSE);

    int code = read_file(path, &config);
    if (code != SUCCESS) return code;

    char **options = NULL;
    ALLOCATE_MEM(options, 1, sizeof(char *));

    char *curr_option = strtok(config, "\n");
    config_s += strlen(curr_option);
    ALLOCATE_MEM(options[0], strlen(curr_option) + 1, sizeof(char));
    strcpy(options[0], curr_option);


    int index = 1;
    int is_found = -1;
    // Save the options.
    while (1) {
        curr_option = strtok(NULL, "\n");
        if (curr_option == NULL) break;

        // Position of the option.
        if (strstr(curr_option, option) != NULL) is_found = index;

        config_s += strlen(curr_option);
        REALLOCATE_MEM(options, (index + 1) * sizeof(char *));
        ALLOCATE_MEM(options[index], strlen(curr_option) + 1, sizeof(char));
        strcpy(options[index], curr_option);
    }
    if (is_found == -1) return FALSE;

    // Here we store the option with the new value.
    char *option_changed_value = str_add(option, ": ", value, NULL);
    // Replace the old
    config_s -= strlen(options[is_found]);
    free(options[is_found]);
    options[is_found] = option_changed_value;

    config_s += strlen(options[is_found]);

    // Rebuild the config.
    char *new_config;
    ALLOCATE_MEM(new_config, config_s + 2, sizeof(char));
    strcpy(new_config, options[0]);
    strcat(new_config, "\n");

    for (int build = 1; build < index + 1; build++) {
        strcat(new_config, options[build]);
        REALLOCATE_MEM(new_config, (strlen(new_config) + 2) * sizeof(char));
        strcat(new_config, "\n");
    }

    code = write_file(path, new_config, config_s + 1);
    if (code != SUCCESS) return code;

    free(new_config);
    FREE_ARRAY(options, index);
    free(config);
    free(path);
    free(option_changed_value);

    return SUCCESS;
}