cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 99)
add_executable(sketch_core main.c tools/file_handler.c tools/include/files/file_handler.h constants/include/codes.h mem/include/mem.h)

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