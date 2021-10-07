/* config_parser.h */
#ifndef SKETCH_CORE_CONFIG_PARSER_H
#define SKETCH_CORE_CONFIG_PARSER_H

/**
 * Check and create the config file if missing.
 * @return SUCCESS or the error number.
 */
extern int create_config_file();

extern int get_option(const char *option, char **result);

extern int set_option(const char *option, const char *value);

extern int set_byte_rate(const char *value);
#endif