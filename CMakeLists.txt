cmake_minimum_required(VERSION 3.5)
project(MyEngine)
file(GLOB_RECURSE sources src/*.cpp)

add_executable(MyEngine ${sources})
target_compile_options(MyEngine PUBLIC -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -Wno-error=old-style-cast)

