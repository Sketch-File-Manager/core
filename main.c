/* main.c */
#include <argument_parser.h>
#include <config_parser.h>
#include <sys/stat.h>
#include <include/codes.h>
#include <include/functions.h>

void create_requirements() {
    char *sketch_folder_loc = fix_path(SKETCH_FOLDER_LOCATION, TRUE);
    char *sketch_core_loc = fix_path(SKETCH_CORE_LOCATION, TRUE);
    char *session_loc = fix_path(SESSION_FOLDER_LOCATION, TRUE);

    // Sketch folder
    mkdir(sketch_folder_loc, 0700);
    // Sketch core folder
    mkdir(sketch_core_loc, 0700);
    // Config file
    create_config_file();
    // Session folder
    mkdir(session_loc, 0700);

    free(sketch_folder_loc);
    free(sketch_core_loc);
    free(session_loc);
}

int main(int argc, char **argv) {
    create_requirements();
    return parse(argc, argv);
}
