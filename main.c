/* main.c */
#ifdef linux
#include <sys/stat.h>
#include <stdlib.h>
#include <codes.h>
#include <functions.h>
#include <parser/argument_parser.h>
#include <parser/config_parser.h>
#endif

#ifndef linux
#include <stdio.h>
#endif

// TODO - Comment for testings.
/*void create_requirements()
{
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
}*/

int main(int argc, char **argv)
{

#ifdef linux
    // create_requirements(); TODO - Comment to test.
    // return parse(argc, argv); TODO - Comment to test.
    return 0;
#else
    printf("The operating system is not compatible.\n");
    return 0;
#endif
}
