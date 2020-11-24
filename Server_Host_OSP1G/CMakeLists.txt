cmake_minimum_required(VERSION 3.17)
project(Server_Host_OSP1G C)

set(CMAKE_C_STANDARD 99)

add_executable(Server_Host_OSP1G proc_structures/proc_structure.h proc_structures/proc_structure.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c modes/sequential.c modes/sequential.h)
#add_executable(Server_Host_OSP1G proc_structures/proc_structure.h proc_structures/proc_structure.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c modes/heavy_process.c modes/heavy_process.h)
#add_executable(Server_Host_OSP1G proc_structures/proc_structure.h proc_structures/proc_structure.c modes/pre_heavy_process.h modes/pre_heavy_process.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c)
TARGET_LINK_LIBRARIES(Server_Host_OSP1G pthread)