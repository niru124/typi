cmake_minimum_required(VERSION 3.10)
project(typi)

set(CMAKE_CXX_STANDARD 17)

# Include the sciplot directory
include_directories(${CMAKE_SOURCE_DIR}/sciplot)

add_executable(typi
    main.cpp
    terminal_utils.cpp
    feedback.cpp
    makegraph.cpp
)
