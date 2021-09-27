#include <string.h>
#include <malloc.h>

#include <argument_parser.h>
#include <session.h>
#include <session_edit.h>
#include <include/codes.h>
#include <include/logger.h>
#include "../commands/commands.h"

#define VERSION                 "1.0.0"

#define COMMAND_NUMBER          26

// simple commands
#define SIMPLE_MKDIR            "do mkdir"
#define SIMPLE_MKFILE           "do mkfile"
#define SIMPLE_COPY             "do copy"
#define SIMPLE_MOVE             "do move"
#define SIMPLE_RENAME           "do rename"
#define SIMPLE_EDIT             "do edit"
#define SIMPLE_PERMISSIONS      "do permissions"
#define SIMPLE_LS               "do ls"

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

// options
#define OPTION_HELP             "--help"
#define OPTION_VERSION_FULL     "--version"

// Function pointer types.
typedef int exec_general(void);

typedef int exec_one_arg(char *);

typedef int exec_two_arg(char *, char *);

typedef int exec_three_arg(char *, char *, char *);

typedef int exec_four_arg(char *, char *, char *, char *);

struct command {
    char *c_name;
    int c_argc;
    exec_general *c_exec;
};

static int print_help() {
    printf("Usage: sketch-core [OPTIONS] COMMAND\n");
    printf("\n");
    printf("A cli file-manager using sessions\n");
    printf("\n");
    printf("Options:\n");
    printf("      --help       Print this help summary page\n");
    printf("  -v, --version    Print version information\n");
    printf("\n");
    printf("Simple Commands:\n");
    printf("All simple commands are executed immediately\n");
    printf("  do mkdir [dst] [name] [permissions]                   Make a directory with [name] and custom [permissions] under the [dst] folder\n");
    printf("  do mkfile [dst] [name] [permissions]                  Make a file with [name] and custom [permissions] under the [dst] folder\n");
    printf("  do copy [src] [dst]                                   Copy [src] (file or directory) under the [dst] folder\n");
    printf("  do move [src] [dst]                                   Move [src] (file or directory) under the [dst] folder\n");
    printf("  do rename [src] [new_name]                            Rename [src] (file or directory) with [new_name]\n");
    printf("  do edit <flag> [src] [content]                        Change the contents of a [src] file\n");
    printf("                                                            Flags: -a, --append_to: appends [content] to the end of the file\n");
    printf("                                                                   -u, --unshift: appends [content] to the start of the file\n");
    printf("                                                                   -w, --write: Discards old content and rewrite it with [content]\n");
    printf("  do permissions [src] [permissions] [recursive]        Change the permissions of [src] (file or directory) to [permissions]\n");
    printf("                                                            If [src] is a directory the [recursive] will be applied (0 or 1).\n");
    printf("                                                                - If [recursive] is 1 then it will change the permissions of all the sub files and directories\n");
    printf("                                                                - Default value for [recursive] is zero (0)\n");
    printf("\n");
    printf("Session Commands:\n");
    printf("Session commands cannot be executed while a session is in use\n");
    printf("  session start             Start a new session the id will be displayed\n");
    printf("  session end [id]          End a session with using its [id]\n");
    printf("  session use [id]          Sets a session with [id] as current session, current session will be editable\n");
    printf("  session run [id]          Runs a session with [id], a session cannot be executed twice\n");
    printf("  session current           Prints the id of the current session\n");
    printf("  session show [id]         Shows the content of a session with [id]\n");
    printf("  session list              List sessions\n");
    printf("\n");
    printf("Session Edit Commands:\n");
    printf("Session edit commands are stored in a file and executed with the write order\n");
    printf("  exit                                              Exit from the current session, the session will not be editable\n");
    printf("  undo                                              Undo the last command\n");
    printf("  mkdir [dst] [name] [permissions]                  Adds the simple commands 'mkdir' to the session\n");
    printf("  mkfile [dst] [name] [permissions]                 Adds the simple commands 'mkfile' to the session\n");
    printf("  copy [src] [dst]                                  Adds the simple commands 'copy' to the session\n");
    printf("  move [src] [dst]                                  Adds the simple commands 'move' to the session\n");
    printf("  rename [src] [new_name]                           Adds the simple commands 'rename' to the session\n");
    printf("  edit <flag> [src] [content]                       Adds the simple commands 'edit' to the session\n");
    printf("  permissions [src] [permissions] [recursive]       Adds the simple commands 'permissions' to the session\n");
    printf("\n");
    printf("Other:\n");
    printf("If a file or directory name contains space then either use \\ before the space or put the directory inside double quotes\n");
    printf("The format for the permissions must be in __mode_t, e.x. 0700\n");
    printf("If the user has insufficient permissions to make the appropriate changes the command will not be executed\n");
    return SUCCESS;
}

static int print_version() {
    printf("Sketch core version %s", VERSION);
    return SUCCESS;
}

static struct command commands[COMMAND_NUMBER] = {
        // simple commands.
        {.c_name = SIMPLE_MKDIR, .c_argc = 3, .c_exec = (exec_general *) command_mkdir},
        {.c_name = SIMPLE_MKFILE, .c_argc = 3, .c_exec = (exec_general *) command_mkfile},
        {.c_name = SIMPLE_COPY, .c_argc = 4, .c_exec = (exec_general *) command_copy},
        {.c_name = SIMPLE_MOVE, .c_argc = 4, .c_exec = (exec_general *) command_move},
        {.c_name = SIMPLE_RENAME, .c_argc = 2, .c_exec = (exec_general *) command_rename},
        {.c_name = SIMPLE_EDIT, .c_argc = 3, .c_exec = (exec_general *) command_edit},
        {.c_name = SIMPLE_PERMISSIONS, .c_argc = 3, .c_exec = (exec_general *) command_permissions},
        {.c_name = SIMPLE_LS, .c_argc = 1, .c_exec = (exec_general *) command_ls},

        // session.
        {.c_name = SESSION_START, .c_argc = 0, .c_exec = (exec_general *) session_start},
        {.c_name = SESSION_END, .c_argc = 1, .c_exec = (exec_general *) session_end},
        {.c_name = SESSION_USE, .c_argc = 1, .c_exec = (exec_general *) session_use},
        {.c_name = SESSION_RUN, .c_argc = 1, .c_exec = (exec_general *) session_run},
        {.c_name = SESSION_CURRENT, .c_argc = 0, .c_exec = (exec_general *) session_current},
        {.c_name = SESSION_SHOW, .c_argc = 1, .c_exec = (exec_general *) session_show},
        {.c_name = SESSION_LIST, .c_argc = 0, .c_exec = (exec_general *) session_list},

        // session commands.
        {.c_name = COMMAND_EXIT, .c_argc = 0, .c_exec = (exec_general *) session_command_exit},
        {.c_name = COMMAND_UNDO, .c_argc = 0, .c_exec = (exec_general *) session_command_undo},
        {.c_name = COMMAND_MKDIR, .c_argc = 3, .c_exec = (exec_general *) session_command_mkdir},
        {.c_name = COMMAND_MKFILE, .c_argc = 3, .c_exec = (exec_general *) session_command_mkfile},
        {.c_name = COMMAND_COPY, .c_argc = 4, .c_exec = (exec_general *) session_command_copy},
        {.c_name = COMMAND_MOVE, .c_argc = 4, .c_exec = (exec_general *) session_command_move},
        {.c_name = COMMAND_RENAME, .c_argc = 2, .c_exec = (exec_general *) session_command_rename},
        {.c_name = COMMAND_EDIT, .c_argc = 3, .c_exec = (exec_general *) session_command_edit},
        {.c_name = COMMAND_PERMISSIONS, .c_argc = 3, .c_exec = (exec_general *) session_command_permission},

        // options
        {.c_name = OPTION_HELP, .c_argc = 0, .c_exec = (exec_general *) print_help},
        {.c_name = OPTION_VERSION_FULL, .c_argc = 0, .c_exec = (exec_general *) print_version}
};

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
    } else {
        index = find_command(argv[1]);
        offset = 1;

        // check if the arguments is enough
        if (commands[index].c_argc > (argc - 2)) {
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
    else if (commands[index].c_argc == 2)
        result = ((exec_two_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset]);
    else if (commands[index].c_argc == 3)
        result = ((exec_three_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset], argv[5 - offset]);
    else if (commands[index].c_argc == 4)
        result = ((exec_four_arg *) commands[index].c_exec)(argv[3 - offset], argv[4 - offset], argv[5 - offset],
                                                            argv[6 - offset]);
    // if is a simple command show some logs.
    if (strcmp(argv[1], "do") == 0 && result != SUCCESS)
        logger(ERROR, "Failed to execute simple command with error code: ", result, NULL);

    return result;
}