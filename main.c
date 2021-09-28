#include <argument_parser.h>
#include <config_parser.h>
#include <sys/stat.h>
#include <include/codes.h>

void create_requirements() {
    // Sketch folder
    mkdir(SKETCH_CORE_LOCATION, 0700);
    // Config file
    create_config_file();
    // Session folder
    mkdir(SESSION_FOLDER_LOCATION, 0700);
}

int main(int argc, char **argv) {
    create_requirements();
    return parse(argc, argv);
}
