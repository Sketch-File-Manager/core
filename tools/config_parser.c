/* config_parser.c */
#include <string.h>

#include <parser/config_parser.h>
#include <files/file_handler.h>
#include <functions.h>
#include <fcntl.h>
#include <unistd.h>
#include <mem.h>


int create_config_file()
{
    char *absolute_path = fix_path(CONFIG_FILE_LOCATION, FALSE);
    size_t config_file_s = strlen(CURRENT_SESSION_ID) + strlen("byte_rate: 516") + 3;

    char config_file[config_file_s];
    sprintf(config_file, "%s%s", CURRENT_SESSION_ID, ":\nbyte_rate: 516\n");

    // try to open the file, and in case that it does not exist create it.
    int config_fd = open(absolute_path, O_CREAT, 0700);
    int result = write_file(absolute_path, config_file, strlen(config_file));
    if (result != SUCCESS) {
        free(absolute_path);
        return result;
    }

    close(config_fd);
    FREE_MEM(absolute_path);

    return SUCCESS;
}

int get_option(const char *option, char **result) {


    return SUCCESS;
}

int set_option(const char *option, const char *value) {


    return SUCCESS;
}