cmake_minimum_required(VERSION 3.10)
project(typi)

set(CMAKE_CXX_STANDARD 17)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Include the sciplot, include, and SQLite3 directories
include_directories(
    ${CMAKE_SOURCE_DIR}/sciplot
    ${CMAKE_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

add_executable(typi
    src/main.cpp
    src/terminal_utils.cpp
    src/feedback.cpp
    src/makegraph.cpp
    src/database.cpp
)

target_link_libraries(typi PRIVATE ${SQLite3_LIBRARIES})