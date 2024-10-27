cmake_minimum_required(VERSION 3.10)
project(GraphCity)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

include_directories(include/)

file(GLOB_RECURSE SRC src/*.c)

set(SOURCE_FILES main.c ${SRC})

# Add the executable
add_executable(app ${SOURCE_FILES})
