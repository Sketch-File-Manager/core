#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "args_parser.h"


#define INVALID      -1
// simple commands.
#define MKDIR         0
#define MKFILE        1
#define COPY          2
#define MOVE          3
#define RENAME        4
#define PERMISSIONS   5
#define LIST          6
#define DELETE        7

// session commands.
#define S_MKDIR       8
#define S_MKFILE      9
#define S_COPY        10
#define S_MOVE        11
#define S_RENAME      12
#define S_PERMISSIONS 13
#define S_LIST        14
#define S_DELETE      15
#define S_UNDO        16

// general porpuse comamnds.
#define SESSION_SET   17
#define BYTE_RATE     18
#define SESSION_KEEP  19

// default values
#define SESSION_FILE  "" // TODO - set the default path of session file.

// command representaion.
struct command 
{
    char *c_name;
    int c_val;
};


// comamnd set
static struct command commands[] = 
{
    {"session-mkdir" ,S_MKDIR    }, {"session-mkfile"     ,S_MKFILE     },
    {"session-copy"  ,S_COPY     }, {"session-move"       ,S_MOVE       },
    {"session-rename",S_RENAME   }, {"session-permissions",S_PERMISSIONS},
    {"session-list"  ,S_LIST     }, {"session-delete"     ,S_DELETE     },
    {"session-undo"  ,S_UNDO     }, {"mkdir"              ,MKDIR        }, 
    {"mkfile"        ,MKFILE     }, {"copy"               ,COPY         }, 
    {"move"          ,MOVE       }, {"rename"             ,RENAME       }, 
    {"permissions"   ,PERMISSIONS}, {"list"               ,LIST         }, 
    {"delete"        ,DELETE     }
};

static struct command option_commands[] =
{
    {"--session=" , SESSION_SET}, {"--session-keep=", SESSION_KEEP},
    {"--byte-rate=", BYTE_RATE }
};

static size_t s_commands = sizeof(commands)/sizeof(struct command); // session commands amount
static size_t s_options  = sizeof(option_commands)/sizeof(struct command); // option commands amount

static inline int lookup_command(const char *c_name, struct command commands[], 
                                 size_t size, int contains)
{
    struct command *tmp;
    for (int i = 0; i < size; i++)
    {
        tmp = &commands[i];

        if (contains)
        {
            if (strstr(tmp->c_name, c_name))
            {
                return tmp->c_val;
            }
        } else 
        {
            if (strcmp(tmp->c_name, c_name) == 0)
            {
                return tmp->c_val;
            }
        }
    }

    return INVALID;
}


static int parse_commands(struct args_parser_args *args, int argc, const char *c_name,
                          char *(*argv[]))
{
    int c_code = lookup_command(c_name, commands,
                                s_commands, 0);
    
    args->command = (char *) c_name;

    if (c_code == MKDIR   || 
        c_code == S_MKDIR ||
        c_code == MKFILE  ||
        c_code == S_MKFILE)
    {
        if (argc < 4) return -1; // check if mkdir/mkfile has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 3);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

        // TODO - check if path is valid?

        // if permissions is missing then.
        if (argc == 4)
        {
            args->command_argv[2] = "700"; // TODO - change it
            (*argv) += 2; // skip mkdir's arguments.
        } else
        {
            args->command_argv[2] = (*argv)[3];
            (*argv) += 3; // skip mkdir's arguments.
        }
    } else if (c_code == COPY     ||
               c_code == S_COPY   ||
               c_code == MOVE     ||
               c_code == S_MOVE   ||
               c_code == RENAME   ||
               c_code == S_RENAME)
    {
        if (argc < 4) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 2);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

    } else if (c_code == PERMISSIONS ||
               c_code == S_PERMISSIONS)
    {
        if (argc < 4) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 2);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run        = 0x1;
        args->command_argv[0] = (strcmp((*argv)[2], ".") == 0)? getenv("PWD") : (*argv)[2]; // if user give . get current directory, otherwise the given path.
        args->command_argv[1] = (*argv)[3];

        // if depth is missing.
        if (argc == 4)
        {
            // TODO - set default depth.
        } else 
        {
            args->command_argv[2] = strstr((*argv)[4], "--depth=");
            if (args->command_argv[2])
            {
                args->command_argv[2] = strstr(args->command_argv[2], "=") + 1; // get the value.
            }
        }

    } else if (c_code == LIST   ||
               c_code == S_LIST ||
               c_code == DELETE ||
               c_code == S_DELETE)
    {
        if (argc < 3) return -1; // check if copy has the right number of arguments.
        args->command_argv = (char **) malloc(sizeof(char *) * 1);
        if (args->command_argv == NULL) return -1; // failed to allocate memory.

        // TODO - check if path is valid?
        args->will_run = 0x1;

        if (argc == 3) // get current path.
        {
            args->command_argv[0] = getenv("PWD"); // if user give . get current directory, otherwise the given path.
        } else
        {
            args->command_argv[0] = (*argv)[2];
        }
    } else if (c_code == S_UNDO) 
    {
        args->will_run = 0x1;
        return 0; // we already has set the name of the command to execute.
    } else
    {
        return -1;
    }

    return 0;
}

static int parse_options(struct args_parser_args *args, int argc, char *c_name,
                         char *(*argv[]))
{
    char *tmp;
    switch(lookup_command(c_name, option_commands,
                          s_options, 1))
    {
        case SESSION_SET:
            tmp = strstr((**argv), "=");
            args->keep = 0x01 & atoi(tmp + 1); // The the value after '='
            break;
        case SESSION_KEEP:
            tmp = strstr((**argv), "=");
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
    }
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
