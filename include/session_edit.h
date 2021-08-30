#ifndef SKETCH_CORE_SESSION_EDIT_H
#define SKETCH_CORE_SESSION_EDIT_H 1

extern void command_exit();

extern void command_undo();

extern void command_mkdir(char* dst, char* folder);

extern void command_mkfile(char* dst, char* file);

extern void command_copy(char* src, char* dst);

extern void command_move(char* src, char* dst);

extern void command_rename(char* src, char* name);

extern void command_edit(char* src, char* content, char* flag);

#endif
