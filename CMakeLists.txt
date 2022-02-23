cmake_minimum_required(VERSION 3.17)
project(corrector VERSION 1.0)

find_package(raylib 4.0 REQUIRED)

file(GLOB SOURCES "src/*.c")

add_executable(${PROJECT_NAME} corrector.c ${SOURCES})
add_executable(asamiya asamiya.c ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} libjson-c.so)
target_link_libraries(${PROJECT_NAME} raylib)

target_include_directories(asamiya PRIVATE include)
target_link_libraries(asamiya m) # Math library
target_link_libraries(asamiya libjson-c.so)
target_link_libraries(asamiya raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()