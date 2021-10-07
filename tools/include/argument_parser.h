/* argument_parser.h */
#ifndef SKETCH_CORE_ARGUMENT_PARSER_H
#define SKETCH_CORE_ARGUMENT_PARSER_H 1

/**
 * An argument parser that calls the designed functions for each acceptable argument.
 * @param argc The argument count.
 * @param argv The array of arguments.
 * @return SUCCESS or the appropriate error number.
 */
int parse(int argc, char **argv);


#endif
