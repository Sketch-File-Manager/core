#include <argument_parser.h>
#include <session.h>
#include <session_edit.h>
#include <string.h>
#include <malloc.h>

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


static command commands[COMMAND_NUMBER] = {
        // session.
        {.c_name = SESSION_START,   .c_argc = 0, .c_session_empty_arg=&session_start},
        {.c_name = SESSION_END,     .c_argc = 1, .c_session_arg=&session_end},
        {.c_name = SESSION_USE,     .c_argc = 1, .c_session_arg=&session_use},
        {.c_name = SESSION_RUN,     .c_argc = 1, .c_session_arg=&session_run},
        {.c_name = SESSION_CURRENT, .c_argc = 0, .c_session_empty_arg=&session_current},
        {.c_name = SESSION_SHOW,    .c_argc = 1, .c_session_arg=&session_show},
        // session commands.
        {.c_name = COMMAND_EXIT,    .c_argc = 0, .c_session_empty_arg=&command_exit},
        {.c_name = COMMAND_UNDO,    .c_argc = 0, .c_session_empty_arg=&command_undo},
        {.c_name = COMMAND_MKDIR,   .c_argc = 2, .c_session_edit_arg=&command_mkdir},
        {.c_name = COMMAND_MKFILE,  .c_argc = 2, .c_session_edit_arg=&command_mkfile},
        {.c_name = COMMAND_COPY,    .c_argc = 2, .c_session_edit_arg=&command_copy},
        {.c_name = COMMAND_MOVE,    .c_argc = 2, .c_session_edit_arg=&command_move},
        {.c_name = COMMAND_RENAME,  .c_argc = 2, .c_session_edit_arg=&command_rename},
        {.c_name = COMMAND_EDIT,    .c_argc = 3, .c_session_edit_multi_arg=&command_edit}
};

static int find_command(char *name) {

    for (int command = 0; command < COMMAND_NUMBER; command++)
        if (strcmp(commands[command].c_name, name) == 0) return command;
    return -1;
}

static int parse_session(char **argv) {
    if (argv[1] == NULL || argv[2] == NULL ) return -1;

    // form the full command.
    char *name = calloc(strlen(argv[1]) + strlen(argv[2]) + 2, sizeof(char));
    strcat(name, argv[1]);
    strcat(name, " ");
    strcat(name, argv[2]);

    int command = find_command(name);
    free(name);
    return command;
}

static int parse_command(char **argv) {
    if (argv[1] == NULL) return -1;

    return find_command(argv[1]);
}

int parse(int argc, char **argv) {

    int command;
    int offset;

    if (strcmp(argv[1], "session") == 0) {
        command = parse_session(argv);
        offset = 0;
        // check if the arguments is enough
        if (commands[command].c_argc > (argc - 3)) return  -1; // TODO call the help function.
    }
    else {
        command = parse_command(argv);
        offset = 1;
        // check if the arguments is enough
        if (commands[command].c_argc > (argc - 2)) return -1; // TODO call the help function here.
    }

    // check if the command exists.
    if (command == -1) return -1; // TODO call the help function here.

    if (commands[command].c_argc == 0) commands[command].c_session_empty_arg();
    else if (commands[command].c_argc == 1) commands[command].c_session_arg(argv[3 - offset]);
    else if (commands[command].c_argc == 2) commands[command].c_session_edit_arg(argv[3 - offset], argv[4 - offset]);
    else if (commands[command].c_argc == 3) commands[command].c_session_edit_multi_arg(argv[3 - offset], argv[4 - offset], argv[5 - offset]);

    return 0;
}