#include <bits/types/FILE.h>
#include <stdio.h>
#include "args_parser.h"
#include "commands.h"
#include "session_utils.h"

static inline __mode_t str_to_mode_t(char *str) {
    return (__mode_t) str;
}

static inline unsigned int str_to_int(char *str) {
    return strtoul(str, NULL, 10);
}

static void decide(struct args_parser_args args) {
    if(args.will_run == 0) return;

    if(!strcmp(args.command, "mkdir"))
        command_mkdir(args.command_argv[0], args.command_argv[1], str_to_mode_t(args.command_argv[2]));
    else if(!strcmp(args.command, "mkfile"))
        command_mkfile(args.command_argv[0], args.command_argv[1], str_to_mode_t(args.command_argv[2]));
    else if(!strcmp(args.command, "copy"))
        command_copy(args.command_argv[0], args.command_argv[1], args.byte_rate);
    else if(!strcmp(args.command, "move"))
        command_move(args.command_argv[0], args.command_argv[1], args.byte_rate);
    else if(!strcmp(args.command, "rename"))
        command_rename(args.command_argv[0], args.command_argv[1]);
    else if(!strcmp(args.command, "perms"))
        command_permissions(str_to_mode_t(args.command_argv[0]), args.command_argv[1]);
    else if(!strcmp(args.command, "perms-recursive"))
        command_permissions_recursive(str_to_mode_t(args.command_argv[0]), args.command_argv[1], str_to_int(args.command_argv[2]));
    else if(!strcmp(args.command, "delete"))
        command_delete(args.command_argv[0]);
    else if(!strcmp(args.command, "list"))
        command_list(args.command_argv[0]);
    else {
        FILE *session_file = fopen(args.session_file, "w+");

        if(!strcmp(args.command, "session-mkdir"))
            session_mkdir(session_file, args.command_argv[0], args.command_argv[1], args.command_argv[2]);
        else if(!strcmp(args.command, "session-mkfile"))
            session_mkfile(session_file, args.command_argv[0], args.command_argv[1], args.command_argv[2]);
        else if(!strcmp(args.command, "session-copy"))
            session_copy(session_file, args.command_argv[0], args.command_argv[1]);
        else if(!strcmp(args.command, "session-move"))
            session_move(session_file, args.command_argv[0], args.command_argv[1]);
        else if(!strcmp(args.command, "session-rename"))
            session_rename(session_file, args.command_argv[0], args.command_argv[1]);
        else if(!strcmp(args.command, "session-perms"))
            session_permissions(session_file, args.command_argv[0], args.command_argv[1]);
        else if(!strcmp(args.command, "session-perms-recursive"))
            session_permissions_recursive(session_file, args.command_argv[0], args.command_argv[1], str_to_int(args.command_argv[2]));
        else if(!strcmp(args.command, "session-delete"))
            session_delete(session_file, args.command_argv[0]);
        else if(!strcmp(args.command, "session-list"))
            session_list(args.session_file, args.command_argv[0]);
        else if(!strcmp(args.command, "session-undo"))
            session_undo(session_file, args.session_file);
        else if(!strcmp(args.command, "session-execute"))
            session_execute(args.session_file, args.keep);

        fclose(session_file);
    }
}

int main(int argc, char **argv) {
    struct args_parser_args args;
    init_args(&args);
    args_parser_parse(&args, argc, argv);
    decide(args);
    return 0;
}
