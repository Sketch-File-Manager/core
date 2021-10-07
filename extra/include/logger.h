/* logger.h */
#ifndef SKETCH_CORE_LOGGER_H
#define SKETCH_CORE_LOGGER_H

#define INFO        0
#define WARNING     1
#define ERROR       2
#define DEBUG       3

/**
 * Logs a message to the stdout.
 * @param priority The priority of the message.
 * @param message The actual message
 * @param ...
 */
extern void logger(int priority, char *message, ...);

#endif //SKETCH_CORE_LOGGER_H
