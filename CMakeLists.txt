cmake_minimum_required(VERSION 3.17)
project(corrector VERSION 1.0)

if(ENV{CROSSCOMP})

    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_VERSION 64)
    set(CMAKE_SYSTEM_PROCESSOR x86)

    set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)

    set(raylib_DIR "/usr/x86_64-w64-mingw32/lib/cmake/raylib")
    set(raylib_INCLUDE_DIR "/usr/x86_64-w64-mingw32/include")
    set(raylib_LIBRARY "/usr/x86_64-w64-mingw32/lib/")
endif()

find_package(raylib 4.0 REQUIRED)
find_package(json-c CONFIG)


file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(corrector corrector.c ${SOURCES})
add_executable(asamiya asamiya.c ${SOURCES})

target_include_directories(corrector PRIVATE include)
target_link_libraries(corrector json-c)
target_link_libraries(corrector raylib)

target_include_directories(asamiya PRIVATE include)
target_link_libraries(asamiya m) # Math library
target_link_libraries(asamiya json-c)
target_link_libraries(asamiya raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
