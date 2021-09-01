#include <string.h>
#include <stdio.h>

#include <config_parser.h>
#include <file_handler.h>


#define CONFIG_PATH        ".local/share/sketch/config.conf"

#define CURRENT_SESSION_ID "current_session:"


int get_current(char** current) {
    char *config = NULL;

    if (read_file(CONFIG_PATH, &config) == -1) return -1;

    /*strtok(location_of_interest, " ");
    char *current_session = strtok(NULL, "\n");

    printf("Current session: %s\n", current_session);
*/
    free(config);
    return 0;
}

int set_current(char* current) {
    return 0;
}
