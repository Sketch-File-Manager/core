#ifndef SKETCH_CORE_SESSION_H
#define SKETCH_CORE_SESSION_H 1


extern void session_start();

extern void session_end(char* id);

extern void session_use(char* id);

extern void session_run(char* id);

extern void session_current();

extern void session_show(char* id);

#endif
