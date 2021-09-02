#include <argument_parser.h>
#include <file_handler.h>



int main(int argc, char **argv) {
    char *config_path = "/.local/share/sketch/config.conf";
    char *sketch_path = "/.local/share/sketch/";
    char *session_path = "/.local/share/sketch/sessions/";

    check_requirements(config_path, sketch_path, session_path);
    parse(argc, argv);
    return 0;
}
