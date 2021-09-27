#include <file_handler.h>
#include <argument_parser.h>


int main(int argc, char **argv) {
    char *config = "config.conf";
    char *sketch = "sketch/";

    check_requirements(config, sketch);
    int result = parse(argc, argv);
    return result;
}
