/* codes.h */
#ifndef SKETCH_CORE_CODES_H
#define SKETCH_CORE_CODES_H

// Error codes
#define SUCCESS 0
#define WRONG_ARGUMENT_COMMAND -1
#define UNKNOWN_COMMAND -2
#define CANNOT_UNDO_ZERO_COMMANDS_FOUND -3

// Boolean types
#define TRUE 1
#define FALSE -1

// Locations
#define SKETCH_FOLDER_LOCATION      "~/.local/share/sketch/"
#define SKETCH_CORE_LOCATION        "~/.local/share/sketch/core/"
#define CONFIG_FILE_LOCATION        "~/.local/share/sketch/core/config.conf"
#define SESSION_FOLDER_LOCATION     "~/.local/share/sketch/core/sessions/"

// Config
#define CURRENT_SESSION_ID          "current_session:"
#define BYTE_RATE                   "byte_rate:"

#endif
