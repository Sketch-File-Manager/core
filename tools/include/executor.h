/* executor.h */
//
// Created by george on 9/1/21.
//

#ifndef SKETCH_CORE_EXECUTOR_H
#define SKETCH_CORE_EXECUTOR_H

/**
 * Executes a command from a session file.
 * @param command The command string.
 * @return SUCCESS in case of success, otherwise the appropriate error number.
 */
extern int execute(char *command);

#endif //SKETCH_CORE_EXECUTOR_H
