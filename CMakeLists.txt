cmake_minimum_required(VERSION 3.7)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRCS main.cpp utils.cpp)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(GL/include)

add_executable(main ${SRCS} GL/src/glad.c)
target_link_libraries(main ${GLFW_LIBRARIES}  dl)


add_custom_command(TARGET main POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders)
