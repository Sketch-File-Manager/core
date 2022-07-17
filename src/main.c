#include "args_parser.h"
#include "commands.h"
#include "session_utils.h"

void decide(struct args_parser_args args) {
    if(args.will_run == 0) return;

    if(!strcmp(args.command, ""))
}

int main(int argc, char **argv) {
    struct args_parser_args args;
    init_args(&args);
    args_parser_parse(&args, argc, argv);
    decide(args);
    return 0;
}