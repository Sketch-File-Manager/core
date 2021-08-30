#ifndef SKETCH_CORE_SESSION_H
#define SKETCH_CORE_SESSION_H 1


extern void session_start(int id);

extern void session_end(int id);

extern void session_use(int id);

extern void session_run(int id);

extern void session_current();

extern void session_show(int id);

#endif
