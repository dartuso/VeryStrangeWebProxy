cmake_minimum_required(VERSION 3.10)
project(a1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(a1
        proxy.c
        proxy_parse.c
        proxy_parse.h)
