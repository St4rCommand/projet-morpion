cmake_minimum_required(VERSION 3.3)
project(projet-morpion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -lpthread")

file(
    GLOB_RECURSE
    SERVEUR_SOURCE_FILES
    morpion-serveur/*
    morpion-outils/*
)

file(
    GLOB_RECURSE
    CLIENT_SOURCE_FILES
    morpion-client/*
    morpion-outils/*
)

add_executable(morpion-serveur ${SERVEUR_SOURCE_FILES})
add_executable(morpion-client ${CLIENT_SOURCE_FILES})

target_link_libraries(morpion-serveur pthread)