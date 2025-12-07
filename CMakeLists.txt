cmake_minimum_required(VERSION 3.31)
project(3drenderer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.0.12/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(3drenderer main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
