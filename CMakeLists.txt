cmake_minimum_required(VERSION 3.19)

project(wayland-outputs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wayland_outputs main.cpp)
target_link_libraries(wayland_outputs PRIVATE -lwayland-client)