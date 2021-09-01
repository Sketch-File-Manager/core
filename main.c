#include <stdio.h>

#include <argument_parser.h>
#include <session_parser.h>


int main(int argc, char **argv) {
    delete_last_line("abc123.session");

    parse(argc, argv);
    return 0;
}
