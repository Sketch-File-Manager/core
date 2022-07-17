#ifndef CORE_ARGS_PARSER_H
#define CORE_ARGS_PARSER_H

#include <memory.h>

struct args_parser_args {
    unsigned int will_run: 1;

    unsigned int byte_rate; /* --byte-rate=516KB */
    char *byte_rate_measure;
    char *session_file; /* --session=session.txt */

    /**
     * mkdir, mkfile, copy, move, rename, perms, perms_recursive, delete, list
     * session_mkdir, session_mkfile, session_copy, session_move, session_rename, session_perms, session_perms_recursive, delete
     * session_list
     */
    char *command;
    char **command_argv;
    unsigned int command_argc;
};

static inline void init_args(struct args_parser_args *args){
    memset(args, 0x0, sizeof(struct args_parser_args));
}

/**
 * Will parse the arguments that was passed when calling the program.
 * @param args The arguments struct.
 */
extern void args_parser_parse(struct args_parser_args *args, int argc, char **argv);

#endif //CORE_ARGS_PARSER_H
