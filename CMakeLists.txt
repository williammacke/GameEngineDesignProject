cmake_minimum_required(VERSION 3.10)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRCS main.cpp utils.cpp models.cpp ModelGen.cpp memMan.cpp Components.cpp Camera.cpp stb_image.cpp)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(GL/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(assimp REQUIRED)

link_directories(${ASSIMP_LIBRARY_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})


add_subdirectory(json)



add_executable(main ${SRCS} GL/src/glad.c)
target_link_libraries(main ${GLFW_LIBRARIES}  dl nlohmann_json::nlohmann_json ${ASSIMP_LIBRARIES})


add_custom_command(TARGET main POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_command(TARGET main POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)
