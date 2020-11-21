cmake_minimum_required(VERSION 3.17)
project(Server_Client_OSP1G C)

set(CMAKE_C_STANDARD 99)

add_executable(Server_Client_OSP1G main.c Validations/image_validations.c Validations/image_validations.h)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
TARGET_LINK_LIBRARIES(Server_Client_OSP1G PRIVATE pthread)
target_link_libraries(Server_Client_OSP1G PRIVATE m)