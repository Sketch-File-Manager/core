#include <stdio.h>

#include <argument_parser.h>
#include <config_parser.h>


int main(int argc, char **argv) {
    char **test = NULL;
    get_current(test);

    parse(argc, argv);
    return 0;
}
