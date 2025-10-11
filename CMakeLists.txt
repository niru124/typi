cmake_minimum_required(VERSION 3.10)
project(typi)

set(CMAKE_CXX_STANDARD 17)

# Include the sciplot and include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/sciplot
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(typi
    src/main.cpp
    src/terminal_utils.cpp
    src/feedback.cpp
    src/makegraph.cpp
)