cmake_minimum_required(VERSION 3.10)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 17)
set(SRC src/Cube.cpp)

find_package(raylib REQUIRED)

add_executable(rubiks_cube ${SRC})
target_link_libraries(rubiks_cube PRIVATE raylib)
