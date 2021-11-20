cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 99)
add_executable(sketch_core main.c sessions/session.c sessions/session_edit.c sessions/include/session.h sessions/include/session_edit.h tools/argument_parser.c tools/include/parser/argument_parser.h tools/config_parser.c tools/include/parser/config_parser.h tools/file_handler.c tools/include/files/file_handler.h tools/session_parser.c tools/include/parser/session_parser.h constants/include/codes.h tools/executor.c tools/include/execute/executor.h "general_functions/functions.c" "general_functions/include/functions.h" commands/commands.c commands/include/commands.h "logger/logger.c" logger/include/logger.h "data_structures/queue.c" "data_structures/include/queue.h" mem/include/mem.h)

include_directories(commands/include
                    constants/include
                    data_structures/include
                    general_functions/include
                    logger/include
                    mem/include
                    sessions/include
                    tools/include
                    )

install(FILES build/sketch-core PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE DESTINATION /usr/bin/)