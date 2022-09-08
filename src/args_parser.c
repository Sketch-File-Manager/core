#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "args_parser.h"


// default values
#define SESSION_FILE  "" // TODO - set the default path of session file.


static int parse_commands(struct args_parser_args *args, int argc, const char *c_name,
                          char *(*argv[]))
{    
    args->command = (char *) c_name;

    if (strcmp(c_name, "mkdir")         || 
        strcmp(c_name, "session-mkdir") ||
        strcmp(c_name, "mkfile")        ||
        strcmp(c_name, "session-mkfile")) {
        if (argc < 4) return -1; // check if mkdir/mkfile has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 3);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

        // TODO - check if path is valid?

        // if permissions is missing then.
        if (argc == 4) {
            args->command_argv[2] = "700"; // TODO - change it
            (*argv) += 2; // skip mkdir's arguments.
        } else {
            args->command_argv[2] = (*argv)[3];
            (*argv) += 3; // skip mkdir's arguments.
        }
    } else if (strcmp(c_name, "copy")         ||
               strcmp(c_name, "session-copy") ||
               strcmp(c_name, "move")         ||
               strcmp(c_name, "session-move") ||
               strcmp(c_name, "rename")       ||
               strcmp(c_name, "session-rename")) {
        if (argc < 4) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 2);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

        // skip arguments.
        (*argv) += 2;

    } else if (strcmp(c_name, "perms") ||
               strcmp(c_name, "session-perms")) {
        if (argc < 4) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 2);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

        // if depth is missing.
        if (argc == 4) {
            // TODO - set default depth.
            // skip arguments.
            (*argv) += 2;
        } else {
            args->command_argv[2] = strstr((*argv)[4], "--depth=");
            if (args->command_argv[2]) {
                args->command_argv[2] = strstr(args->command_argv[2], "=") + 1; // get the value.
            } else {
                return -1;
            }
            (*argv) += 3;
        }

    } else if (strcmp(c_name, "list")         ||
               strcmp(c_name, "session-list") ||
               strcmp(c_name, "delete")       ||
               strcmp(c_name, "session-delete")) {
        if (argc < 3) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 1);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run = 0x1;

        if (argc == 3) { // get current path.
            args->command_argv[0] = getenv("PWD"); // if user give . get current directory, otherwise the given path.
        } else {
            args->command_argv[0] = (*argv)[2];
            ++(*argv);
        }
    } else if (strcmp(c_name, "undo")) {
        args->will_run = 0x1;
        return 0; // we already has set the name of the command to execute.
    } else {
        return -1;
    }

    return 0;
}

static int parse_options(struct args_parser_args *args, int argc, char *c_name,
                         char *(*argv[]))
{

    

    /*char *tmp;
    switch(lookup_command(c_name, option_commands,
                          s_options, 1))
    {
        case SESSION_SET:
            tmp = strstr((**argv), "=");
            args->keep = 0x01 & atoi(tmp + 1); // The the value after '='
            break;
        case SESSION_KEEP:
            args->keep = 0x01 & atoi(tmp + 1); // The the value after '='
            break;
        case BYTE_RATE:
            tmp = strstr((**argv), "=");
            args->byte_rate_measure = tmp + 1; // get the value.
            args->byte_rate         = (unsigned int) atoi(tmp + 1); // get integer value.
            break;
        case INVALID:
            return -1;
        defualt:
            return -1;
    }*/
    return 0;

}


void args_parser_parse(struct args_parser_args *args, int argc, char *argv[])
{
    args->will_run = 0x0;
    args->keep     = 0x0;

    if (argc < 2) return;

    char **tmp = ++argv;

    while (*tmp)
    {
        if (parse_commands(args, argc, *tmp, &tmp) == 0)
        {
            if (args->will_run == 1) break; // do not run second simple/session command.

            tmp = (tmp == NULL)? tmp + 1 : NULL;
            continue;
        } else if (parse_options(args, argc, *tmp, &tmp) == 0)
        {
            tmp = (tmp == NULL)? tmp + 1 : NULL;
            continue; // run as much option commands as the user need.
        }
        
        return;
    }

    // set options to default values.
    if (args->session_file == NULL)
    {
        args->session_file = (char *) malloc(sizeof(char) * strlen(SESSION_FILE) + 1);
        strcpy(args->session_file, SESSION_FILE);
    } 
    if (args->byte_rate == 0)
    {
        args->byte_rate         =  516;
        args->byte_rate_measure = "516";
    } 
}
