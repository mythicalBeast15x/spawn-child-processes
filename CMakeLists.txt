cmake_minimum_required(VERSION 3.27)
project(spawn_child_processes C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(spawn_child_processes
        main.c
        main.h)
