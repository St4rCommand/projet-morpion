cmake_minimum_required(VERSION 3.3)
project(projet-morpion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES morpion-serveur/morpion-serveur.c)
add_executable(untitled1 ${SOURCE_FILES})