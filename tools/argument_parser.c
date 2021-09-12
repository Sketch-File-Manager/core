#include <string.h>
#include <malloc.h>

#include <argument_parser.h>
#include <session.h>
#include <session_edit.h>
#include <codes.h>
#include <logger.h>
#include "../commands/commands.h"

#define COMMAND_NUMBER 23

// simple commands
#define SIMPLE_MKDIR           "do mkdir"
#define SIMPLE_MKFILE          "do mkfile"
#define SIMPLE_COPY            "do copy"
#define SIMPLE_MOVE            "do move"
#define SIMPLE_RENAME          "do rename"
#define SIMPLE_EDIT            "do edit"
#define SIMPLE_PERMISSIONS     "do permissions"

// session commands.
#define SESSION_START           "session start"
#define SESSION_END             "session end"
#define SESSION_USE             "session use"
#define SESSION_RUN             "session run"
#define SESSION_CURRENT         "session current"
#define SESSION_SHOW            "session show"
#define SESSION_LIST            "session list"

// session edit commands.
#define COMMAND_EXIT            "exit"
#define COMMAND_UNDO            "undo"
#define COMMAND_MKDIR           "mkdir"
#define COMMAND_MKFILE          "mkfile"
#define COMMAND_COPY            "copy"
#define COMMAND_MOVE            "move"
#define COMMAND_RENAME          "rename"
#define COMMAND_EDIT            "edit"
#define COMMAND_PERMISSIONS     "permissions"

// Function pointer types.
typedef int exec_general(void);
typedef int exec_one_arg(char *);
typedef int exec_two_arg(char *, char *);
typedef int exec_three_arg(char *, char *, char *);
typedef int exec_four_arg(char *, char *, char *, char *);

struct command {
    char         *c_name;
    int           c_argc;
    exec_general *c_exec;
};

static struct command commands[COMMAND_NUMBER] = {
        // simple commands.
        {.c_name = SIMPLE_MKDIR,       .c_argc = 3, .c_exec = (exec_general *) command_mkdir},
        {.c_name = SIMPLE_MKFILE,      .c_argc = 3, .c_exec = (exec_general *) command_mkfile},
        {.c_name = SIMPLE_COPY,        .c_argc = 4, .c_exec = (exec_general *) command_copy},
        {.c_name = SIMPLE_MOVE,        .c_argc = 4, .c_exec = (exec_general *) command_move},
        {.c_name = SIMPLE_RENAME,      .c_argc = 2, .c_exec = (exec_general *) command_rename},
        {.c_name = SIMPLE_EDIT,        .c_argc = 3, .c_exec = (exec_general *) command_edit},
        {.c_name = SIMPLE_PERMISSIONS, .c_argc = 3, .c_exec = (exec_general *) command_permissions},

        // session.
        {.c_name = SESSION_START,       .c_argc = 0, .c_exec = (exec_general *) session_start},
        {.c_name = SESSION_END,         .c_argc = 1, .c_exec = (exec_general *) session_end},
        {.c_name = SESSION_USE,         .c_argc = 1, .c_exec = (exec_general *) session_use},
        {.c_name = SESSION_RUN,         .c_argc = 1, .c_exec = (exec_general *) session_run},
        {.c_name = SESSION_CURRENT,     .c_argc = 0, .c_exec = (exec_general *) session_current},
        {.c_name = SESSION_SHOW,        .c_argc = 1, .c_exec = (exec_general *) session_show},
        {.c_name = SESSION_LIST,        .c_argc = 0, .c_exec = (exec_general *) session_list},

        // session commands.
        {.c_name = COMMAND_EXIT,        .c_argc = 0, .c_exec = (exec_general *) session_command_exit},
        {.c_name = COMMAND_UNDO,        .c_argc = 0, .c_exec = (exec_general *) session_command_undo},
        {.c_name = COMMAND_MKDIR,       .c_argc = 3, .c_exec = (exec_general *) session_command_mkdir},
        {.c_name = COMMAND_MKFILE,      .c_argc = 3, .c_exec = (exec_general *) session_command_mkfile},
        {.c_name = COMMAND_COPY,        .c_argc = 4, .c_exec = (exec_general *) session_command_copy},
        {.c_name = COMMAND_MOVE,        .c_argc = 4, .c_exec = (exec_general *) session_command_move},
        {.c_name = COMMAND_RENAME,      .c_argc = 2, .c_exec = (exec_general *) session_command_rename},
        {.c_name = COMMAND_EDIT,        .c_argc = 3, .c_exec = (exec_general *) session_command_edit},
        {.c_name = COMMAND_PERMISSIONS, .c_argc = 3, .c_exec = (exec_general *) session_command_permission}
};

static void print_help() {
    // TODO - print help table
}

static inline int find_command(char *name) {
    for (int curr_command = 0; curr_command < COMMAND_NUMBER; curr_command++)
        if (strcmp(commands[curr_command].c_name, name) == 0) return curr_command;
    return -1;
}

static inline int parse_double(char **argv) {
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

    int index, offset;
    if (strcmp(argv[1], "session") == 0 || strcmp(argv[1], "do") == 0) {
        index = parse_double(argv);
        offset = 0;

        // check if the arguments is enough
        if (commands[index].c_argc > (argc - 3)) {
            print_help();
            return WRONG_ARGUMENT_COMMAND;
        }
    }
    else {
        index = find_command(argv[1]);
        offset = 1;

        // check if the arguments is enough
        if (commands[index].c_argc > (argc - 2))  {
            print_help();
            return WRONG_ARGUMENT_COMMAND;
        }
    }

    // check if the index exists.
    if (index == -1) {
        print_help();
        return WRONG_ARGUMENT_COMMAND;
    }

    int result = SUCCESS;
    if (commands[index].c_argc == 0) result = commands[index].c_exec();
    else if (commands[index].c_argc == 1) result = ((exec_one_arg *) commands[index].c_exec)(argv[3 - offset]);
    else if (commands[index].c_argc == 2) result = ((exec_two_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset]);
    else if (commands[index].c_argc == 3) result = ((exec_three_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset], argv[5 - offset]);
    else if (commands[index].c_argc == 4) result = ((exec_four_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset], argv[5 - offset], argv[6 - offset]);

    // if is a simple command show some logs.
    if(strcmp(argv[1], "do") == 0)
        log(ERROR, "Failed to execute simple command with error code: ", result);

    return result;
}