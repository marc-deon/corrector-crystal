cmake_minimum_required(VERSION 3.17)
project(corrector_crystal VERSION 1.0)

# git clone https://github.com/aminosbh/sdl2-cmake-modules cmake/sdl2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF SDL2::Mixer)

target_link_libraries(${PROJECT_NAME} libjson-c.so)

# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})