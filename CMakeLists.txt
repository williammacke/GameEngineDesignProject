cmake_minimum_required(VERSION 3.7)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(GL/include)

add_executable(main main.cpp GL/src/glad.c)
target_link_libraries(main ${GLFW_LIBRARIES}  dl)
