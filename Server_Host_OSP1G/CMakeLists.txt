cmake_minimum_required(VERSION 3.17)
project(Server_Host_OSP1G C)

set(CMAKE_C_STANDARD 99)

add_executable(Server_Host_OSP1G main.c proc_structures/proc_structure.h proc_structures/proc_structure.c modes/delegation_mode.h modes/delegation_mode.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c)
TARGET_LINK_LIBRARIES(Server_Host_OSP1G pthread)