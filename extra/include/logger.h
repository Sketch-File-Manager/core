#ifndef SKETCH_CORE_LOGGER_H
#define SKETCH_CORE_LOGGER_H

#define INFO 0
#define WARNING 1
#define ERROR 2
#define DEBUG 3

extern void logger(int priority, char* message, ...);

#endif //SKETCH_CORE_LOGGER_H
