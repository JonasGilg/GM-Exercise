cmake_minimum_required(VERSION 3.10)

project(GM-Exercise)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if (WIN32)
    message("Using WIN32")

    file(TO_CMAKE_PATH $ENV{CPP_LIBS} C++LIBS)
    if (MINGW)
        message("Using MinGW")
        set(GLUT_glut_LIBRARY "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/lib/libfreeglut.a")
        set(GLUT_INCLUDE_DIR "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/include")
    elseif (MSVC)
        message("Using MSVC")
        set(GLUT_glut_LIBRARY "{C++LIBS}/msvc/freeglut/freeglut-3.0.0/lib/libfreeglut.lib")
        set(GLUT_INCLUDE_DIR "${C++LIBS}/msvc/freeglut/freeglut-3.0.0/include")
    endif ()
endif ()
find_package(GLUT REQUIRED)

message("")
message("GLUT_FOUND: ${GLUT_FOUND}")
message("GLUT_glut_LIBRARY: ${GLUT_glut_LIBRARY}")
message("GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")

if (WIN32)
    set(GLM_INCLUDE_DIR "${C++LIBS}/all/glm/glm-0.9.9.0")
    find_package(GLM REQUIRED)
endif ()

message("")
message("GLM_FOUND: ${GLM_FOUND}")
message("GLM_INCLUDE_DIR: ${GLM_INCLUDE_DIR}")
message("")

include_directories(${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

add_executable(gm_exercise gmuebung.cpp BezierCurve.cpp BezierCurve.h AxisAlignedBoundingBox.cpp AxisAlignedBoundingBox.h)

target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})