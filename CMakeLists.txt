cmake_minimum_required(VERSION 3.10)
project(typi)

set(CMAKE_CXX_STANDARD 17)

# Include the sciplot directory
include_directories(${CMAKE_SOURCE_DIR}/sciplot)

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG        v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR})
endif()


add_executable(typi
    main.cpp
    terminal_utils.cpp
    makegraph.cpp
)

target_link_libraries(typi ftxui::screen ftxui::dom ftxui::component)
