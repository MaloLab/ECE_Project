cmake_minimum_required(VERSION 3.29)
project(optim C)

set(CMAKE_C_STANDARD 99)

add_executable(optim main.c
        Interface.c header.h)
