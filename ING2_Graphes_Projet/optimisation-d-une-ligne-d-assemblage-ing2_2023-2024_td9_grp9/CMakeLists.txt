cmake_minimum_required(VERSION 3.25)
project(optimisation_d_une_ligne_d_assemblage_ing2_2023_2024_td9_grp9 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(optimisation_d_une_ligne_d_assemblage_ing2_2023_2024_td9_grp9
        header.h
        main.c
        Interface.c)
