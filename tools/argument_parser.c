#include <string.h>
#include <malloc.h>

#include <argument_parser.h>
#include <session.h>
#include <session_edit.h>

#define COMMAND_NUMBER 14

// session commands.
#define SESSION_START     "session start"
#define SESSION_END       "session end"
#define SESSION_USE       "session use"
#define SESSION_RUN       "session run"
#define SESSION_CURRENT   "session current"
#define SESSION_SHOW      "session show"

// session edit commands.
#define COMMAND_EXIT      "exit"
#define COMMAND_UNDO      "undo"
#define COMMAND_MKDIR     "mkdir"
#define COMMAND_MKFILE    "mkfile"
#define COMMAND_COPY      "copy"
#define COMMAND_MOVE      "move"
#define COMMAND_RENAME    "rename"
#define COMMAND_EDIT      "edit"

// Function pointer types.
typedef void exec_general(void);
typedef void exec_one_arg(char *);
typedef void exec_two_arg(char *, char *);
typedef void exec_three_arg(char *, char *, char *);

struct command {
    char         *c_name;
    int           c_argc;
    exec_general *c_exec;
};

static struct command commands[COMMAND_NUMBER] = {
        // session.
        {.c_name = SESSION_START,   .c_argc = 0, .c_exec = (exec_general *) session_start},
        {.c_name = SESSION_END,     .c_argc = 1, .c_exec = (exec_general *) session_end},
        {.c_name = SESSION_USE,     .c_argc = 1, .c_exec = (exec_general *) session_use},
        {.c_name = SESSION_RUN,     .c_argc = 1, .c_exec = (exec_general *) session_run},
        {.c_name = SESSION_CURRENT, .c_argc = 0, .c_exec = (exec_general *) session_current},
        {.c_name = SESSION_SHOW,    .c_argc = 1, .c_exec = (exec_general *) session_show},
        // session commands.
        {.c_name = COMMAND_EXIT,    .c_argc = 0, .c_exec = (exec_general *) command_exit},
        {.c_name = COMMAND_UNDO,    .c_argc = 0, .c_exec = (exec_general *) command_undo},
        {.c_name = COMMAND_MKDIR,   .c_argc = 2, .c_exec = (exec_general *) command_mkdir},
        {.c_name = COMMAND_MKFILE,  .c_argc = 2, .c_exec = (exec_general *) command_mkfile},
        {.c_name = COMMAND_COPY,    .c_argc = 2, .c_exec = (exec_general *) command_copy},
        {.c_name = COMMAND_MOVE,    .c_argc = 2, .c_exec = (exec_general *) command_move},
        {.c_name = COMMAND_RENAME,  .c_argc = 2, .c_exec = (exec_general *) command_rename},
        {.c_name = COMMAND_EDIT,    .c_argc = 3, .c_exec = (exec_general *) command_edit}
};

static inline int find_command(char *name) {

    for (int curr_command = 0; curr_command < COMMAND_NUMBER; curr_command++)
        if (strcmp(commands[curr_command].c_name, name) == 0) return curr_command;
    return -1;
}

static inline int parse_session(char **argv) {
    if (argv[1] == NULL || argv[2] == NULL) return -1;

    // form the full result_command.
    char *name = calloc(strlen(argv[1]) + strlen(argv[2]) + 2, sizeof(char));
    strcat(name, argv[1]);
    strcat(name, " ");
    strcat(name, argv[2]);

    int result_command = find_command(name);
    free(name);
    return result_command;
}

int parse(int argc, char **argv) {
    if (argv[1] == NULL) return -1;

    int result_command;
    int offset;

    if (strcmp(argv[1], "session") == 0) {
        result_command = parse_session(argv);
        offset = 0;
        // check if the arguments is enough
        if (commands[result_command].c_argc > (argc - 3)) return  -1; // TODO call the help function.
    }
    else {
        result_command = find_command(argv[1]);
        offset = 1;
        // check if the arguments is enough
        if (commands[result_command].c_argc > (argc - 2)) return -1; // TODO call the help function here.
    }

    // check if the result_command exists.
    if (result_command == -1) return -1; // TODO call the help function here.

    if (commands[result_command].c_argc == 0) commands[result_command].c_exec();
    else if (commands[result_command].c_argc == 1)  ((exec_one_arg *) commands[result_command].c_exec)(argv[3 - offset]);
    else if (commands[result_command].c_argc == 2)  ((exec_two_arg *) commands[result_command].c_exec)(argv[3 - offset], argv[4 - offset]);
    else if (commands[result_command].c_argc == 3)  ((exec_three_arg *) commands[result_command].c_exec)(argv[3 - offset], argv[4 - offset], argv[5 - offset]);

    return 0;
}