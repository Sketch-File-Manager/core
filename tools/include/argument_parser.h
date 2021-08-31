#ifndef SKETCH_CORE_ARGUMENT_PARSER_H
#define SKETCH_CORE_ARGUMENT_PARSER_H 1

typedef struct command {
    char *c_name;
    int   c_argc;
    void (*c_session_arg)(char *);
    void (*c_session_edit_arg)(char *, char *);
    void (*c_session_edit_multi_arg)(char *, char *, char *);
    void (*c_session_empty_arg)();
} command;


int parse(int argc, char **argv);


#endif
