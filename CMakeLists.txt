cmake_minimum_required(VERSION 3.10)
project(typing_test)

set(CMAKE_CXX_STANDARD 17)

# Include the sciplot directory
include_directories(${CMAKE_SOURCE_DIR}/sciplot)

add_executable(typing_test
    main.cpp
    terminal_utils.cpp
    feedback.cpp
    makegraph.cpp
)
