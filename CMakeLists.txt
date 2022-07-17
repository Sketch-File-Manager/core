cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 17)

include_directories(include)

SET(SOURCES src/main.c src/commands.c src/args_parser.c src/session_utils.c)
SET(HEADERS include/commands.h src/args_parser.h include/session_utils.h)



add_executable(sketch_core ${SOURCES} ${HEADERS})
