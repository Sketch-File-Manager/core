cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 99)
add_executable(sketch_core main.c sessions/session.c sessions/session_edit.c include/session.h include/session_edit.h tools/argument_parser.c tools/include/argument_parser.h tools/config_parser.c tools/include/config_parser.h tools/file_handler.c tools/include/file_handler.h tools/session_parser.c tools/include/session_parser.h extra/codes.h tools/executor.c tools/include/executor.h extra/functions.c extra/functions.h commands/commands.c commands/commands.h extra/logger.c extra/logger.h extra/directory_queue.c extra/directory_queue.h)

include_directories(include)
include_directories(tools/include)
include_directories(extra)
include_directories(commands/)

install(FILES build/sketch-core PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE DESTINATION /usr/bin/)