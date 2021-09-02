#include <stdio.h>

#include <argument_parser.h>
#include <session_parser.h>
#include <file_handler.h>

int main(int argc, char **argv) {
    check_requirements();
    parse(argc, argv);
    return 0;
}
