#ifndef CORE_ARGS_PARSER_H
#define CORE_ARGS_PARSER_H

#include <memory.h>
#include <bits/types.h>

struct args_parser_args 
{ 
    char *byte_rate_measure;
    char *session_file; /* --session=sketch-core.session */

    /**
     * mkdir, mkfile, copy, move, rename, perms, perms-recursive, delete, list
     * session-mkdir, session-mkfile, session-copy, session-move, session-rename, session-perms, session-perms-recursive, delete
     * session-list, session-undo, session-execute
     */
    char *command;
    char **command_argv;
    unsigned int command_argc;
    unsigned int byte_rate; /* --byte-rate=516KB */
    unsigned int keep: 1; /* --seesion-keep */
    unsigned int will_run: 1;
};

static inline void init_args(struct args_parser_args *args) 
{
    memset(args, 0x0, sizeof(struct args_parser_args));
}

static inline void args_parser_free_argv(struct args_parser_args *args)
{
    free(args->command_argv);
}

/**
 * Will parse the arguments that was passed when calling the program.
 * @param args The arguments struct.
 */
extern void args_parser_parse(struct args_parser_args *args, int argc, char *argv[]);

#endif
