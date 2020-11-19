cmake_minimum_required(VERSION 3.17)
project(Server_Client_OSP1G C)

set(CMAKE_C_STANDARD 99)

add_executable(Server_Client_OSP1G main.c)
TARGET_LINK_LIBRARIES(Server_Client_OSP1G pthread)