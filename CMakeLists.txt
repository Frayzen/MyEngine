cmake_minimum_required(VERSION 3.11)
project(MyEngine)

set(OpenGL_GL_PREFERENCE GLVND)

# DEPS

include(cmake/Deps.cmake)

SetupDeps()

find_package(OpenGL REQUIRED)
# find_package(assimp REQUIRED)
# find_package(glfw3 REQUIRED)
# find_package(glm REQUIRED)

# ENGINE TARGET

file(GLOB_RECURSE cpp_sources src/*.cc)
file(GLOB_RECURSE c_sources src/*.c)

add_executable(MyEngine ${sources} ${cpp_sources})
target_compile_options(MyEngine PUBLIC -Wall -Wextra -Werror -pedantic -std=c++20)

target_include_directories(MyEngine PUBLIC ./include)

target_link_libraries(MyEngine OpenGL::GL)
target_link_libraries(MyEngine assimp)
target_link_libraries(MyEngine glm::glm)
target_link_libraries(MyEngine glfw)
target_link_libraries(MyEngine imgui)

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  ADD_CUSTOM_TARGET(link_target ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
endif()
