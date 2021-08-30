cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 99)
include_directories(include)
include_directories(tools/include)

add_executable(sketch_core main.c tools/argument_parser.c sessions/session.c sessions/session_edit.c include/session.h include/session_edit.h)
