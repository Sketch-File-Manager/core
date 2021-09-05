#ifndef SKETCH_CORE_CODES_H
#define SKETCH_CORE_CODES_H

// Error codes
#define SUCCESS 0
#define UNKNOWN_COMMAND -1


// Boolean types
#define TRUE 1
#define FALSE -1

// Console messages (Will be change, so it will be easy to use grep command).
#define SESSION_IN_USE                      "Session is already in use."

#define MSG_ERROR_CODE                      "Errno:"
#define MSG_SESSION_CURRENT_NOT_FOUND       "Cannot get current session."

#define MSG_SESSION_CREATED_SUCCESSFULLY    "Session created successfully.\nSession id:"
#define MSG_SESSION_FAILED_TO_CREATE        "Failed to create session."

#define MSG_SESSION_ENDED                   "Session ended."
#define MSG_SESSION_FAILED_TO_END           "Session failed to end."

#define MSG_SESSION_USE_SUCCESSFUL          "Current session:"
#define MSG_SESSION_FAILED_TO_USE           "Failed to use session."

#define MSG_SESSION_DOES_NOT_EXIST          "Session does not exist."

#define MSG_SESSION_CANNOT_EXECUTE          "Cannot execute session."
#define MSG_SESSION_ABORT_REST              "aborting the rest of the session."
#define MSG_SESSION_ALREADY_EXECUTED        "Session is already executed."
#define MSG_SESSION_CANNOT_OPEN             "Cannot read session."

#define MSG_NO_CURRENT_SESSION              "No current session is defined."

#define MSG_EXIT_CURRENT_SESSION            "Exited current session."
#define MSG_CANNOT_EXIT_CURRENT_SESSION     "Cannot exit current session."

#define MSG_CANNOT_UNDO_COMMAND             "Cannot undo command in current session."

#define MSG_CANNOT_ADD_COMMAND              "Cannot add command to current session."

#define MSG_INVALID_RECURSIVE_OPTION        "Recursive must be 0 or 1."

#endif
