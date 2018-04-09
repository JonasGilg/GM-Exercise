cmake_minimum_required(VERSION 3.10)

project(GM-Exercise)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if(WIN32)
    set(GLUT_glut_LIBRARY "D:/C++Libs/mingw/freeglut/freeglut-3.0.0/lib/libfreeglut.a")
    set(GLUT_INCLUDE_DIR "D:/C++Libs/mingw/freeglut/freeglut-3.0.0/include")
endif()
find_package(GLUT REQUIRED)

if(WIN32)
    set(GLM_INCLUDE_DIR "D:/C++Libs/all/glm/glm-0.9.8.5")
    find_package(GLM REQUIRED)
endif()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

add_executable(gm_exercise gmuebung.cpp BezierCurve.cpp BezierCurve.h)

target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})