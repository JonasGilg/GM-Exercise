cmake_minimum_required(VERSION 3.10)

project(GM-Exercise)

set(CMAKE_CXX_STANDARD 17)

add_executable(gm_exercise gmuebung.cpp Array.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})