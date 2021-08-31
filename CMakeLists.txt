cmake_minimum_required(VERSION 3.20)
project(sketch_core C)

set(CMAKE_C_STANDARD 99)
add_executable(sketch_core main.c sessions/session.c sessions/session_edit.c include/session.h include/session_edit.h tools/argument_parser.c tools/include/argument_parser.h tools/config_parser.c tools/include/config_parser.h)

include_directories(include)
include_directories(tools/include)

file(MAKE_DIRECTORY /home/$ENV{USER}/.local/share/sketch/)
file(MAKE_DIRECTORY /home/$ENV{USER}/.local/share/sketch/sessions)
file(MAKE_DIRECTORY config/)

file(WRITE config/config.conf)

install(FILES config/config.conf PERMISSIONS OWNER_WRITE OWNER_READ GROUP_WRITE GROUP_READ WORLD_WRITE WORLD_READ DESTINATION /home/$ENV{USER}/.local/share/sketch/config/)
install(FILES build/sketch-core PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE DESTINATION /usr/bin/)