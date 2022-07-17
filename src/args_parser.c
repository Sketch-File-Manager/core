#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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


// command representaion.
struct command 
{
    char *c_name;
    int c_val;
};

// simple command.
static struct command simple_commands[] = 
{
    {"mkdir",  MKDIR }, {"mkfile"     ,  MKFILE     },
    {"copy" ,  COPY  }, {"move"       ,  MOVE       },
    {"rename", RENAME}, {"permissions",  PERMISSIONS},
    {"list",   LIST  }, {"delete"     ,  DELETE     }
};

// session comamnds.
static struct command session_commands[] = 
{
    {"session-mkdir" , S_MKDIR }, {"session-mkfile"     ,  S_MKFILE     },
    {"session-copy"  , S_COPY  }, {"session-move"       ,  S_MOVE       },
    {"session-rename", S_RENAME}, {"session-permissions",  S_PERMISSIONS},
    {"session-list"  , S_LIST  }, {"session-delete"     ,  S_DELETE     },
    {"session-undo"  , S_UNDO  }
};

static struct command option_commands[] =
{
    {"--session=" , SESSION_SET}, {"--session-keep=", SESSION_KEEP},
    {"--byte-rate=", BYTE_RATE  }
};

static size_t s_simple  = sizeof(simple_commands)/sizeof(struct command); // simple commands amount
static size_t s_session = sizeof(session_commands)/sizeof(struct command); // session commands amount
static size_t s_options = sizeof(option_commands)/sizeof(struct command); // option commands amount

static inline int lookup_command(const char *c_name, struct command commands[], 
                                 size_t size, int contains)
{
    struct command *tmp;
    for (int i = 0; i < size; i++)
    {
        tmp = &commands[i];
        if (contains)
        {
            if (strstr(tmp->c_name, c_name) == 0)
            {
                return tmp->c_val;
            }
        }
        else 
        {
            if (strcmp(tmp->c_name, c_name) == 0)
            {
                return tmp->c_val;
            }
        }
    }

    return INVALID;
}


static int parse_session_commands(struct args_parser_args *args, int argc, const char *c_name,
                                  char *(*argv[]))
{

    switch(lookup_command(c_name, session_commands,
                          s_session, 0))
    {
        case S_MKDIR:
            if ( argc < 3) return -1; // check if mkdir has the right amount arguments.

            args->command      = (char *) c_name;
            args->command_argv = (char **) malloc(sizeof(char *) * 3);
            if (args->command_argv == NULL) return -1; // failed to allocate memory.

            args->command_argv[0] = getenv("PWD"); // current directory.
            args->command_argv[1] = (*argv)[2];

            // if permissions is missing then.
            if (argc == 3)
                args->command_argv[2] = "700"; // TODO - change it
            else
                args->command_argv[2] = (*argv)[3];

            break;
        case S_MKFILE:

            
            break;
        case S_COPY:
           
            break;
        case S_MOVE:
            
            break;
        case S_RENAME:
            
            break;
        case S_PERMISSIONS:
           
            break;
        case S_LIST:
            
            break;
        case S_DELETE:
           
            break;
        case S_UNDO:
           
            break;
        defualt:
            
            return -1;
    }
    return 0;
}

static int parse_simple_commands(struct args_parser_args *args, int argc, char *c_name,
                                 char *(*argv[]))
{
    
    switch(lookup_command(c_name, simple_commands,
                          s_simple, 0))
    {
        case MKDIR:

            break;
        case MKFILE:
            
            break;
        case COPY:
           
            break;
        case MOVE:
            
            break;
        case RENAME:
            
            break;
        case PERMISSIONS:
           
            break;
        case LIST:
            
            break;
        case DELETE:
           
            break;

        defualt:
            
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
            args->byte_rate = (unsigned int) atoi(tmp + 1); // get integer value.
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
    args->keep = 0x0;

    if (argc < 2) return;

    char **tmp = ++argv;
    while (*tmp)
    {
        if (parse_simple_commands(args, argc, *tmp, &tmp) == 0 ||
            parse_session_commands(args, argc, *tmp, &tmp) == 0)
        {
            if (args->will_run == 1) break; // do not run second simple/session command.

            ++tmp;
            continue;
        }
        else if (parse_options(args, argc, *tmp, &tmp) == 0)
        {
            ++tmp;
            continue; // run as much option commands as the user need.
        }
        
        return;
    }
}
