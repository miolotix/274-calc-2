cmake_minimum_required(VERSION 3.15)
project(calc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

add_executable(prog5 prog5.c definition.c)
target_link_libraries(prog5 m)