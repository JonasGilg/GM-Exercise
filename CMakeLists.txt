cmake_minimum_required(VERSION 3.10)

project(GM-Exercise)
set(CMAKE_CXX_STANDARD 17)

########################################################################################################################
### SETUP FOR WINDOWS ##################################################################################################

message("SYSTEM AND BUILD INFORMATION:")
if (WIN32)
    message(STATUS "Platform: Windows")

    # Check if 64bit is enabled
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(64BIT "TRUE")
        message(STATUS "64bit")
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(64BIT "FALSE")
        message(STATUS "32bit")
    endif ()

    # Get Path to C++ library directory and store it in "C++LIBS" variable
    file(TO_CMAKE_PATH $ENV{CPP_LIBS} C++LIBS)

    # Check toolchain, 32/64bit and set variables accordingly
    if (MINGW)
        message(STATUS "Toolchain: MinGW")

        if (64BIT)
            # GLUT 64
            set(GLUT_glut_LIBRARY "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/lib/x64/libfreeglut.a")
            set(GLUT_INCLUDE_DIR "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/include")
        else ()
            # GLUT 32
            set(GLUT_glut_LIBRARY "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/lib/libfreeglut.a")
            set(GLUT_INCLUDE_DIR "${C++LIBS}/mingw/freeglut/freeglut-3.0.0/include")
        endif ()
    elseif (MSVC)
        message(STATUS "Toolchain: MSVC")

        if (64BIT)
            # GLUT 64
            set(GLUT_glut_LIBRARY "{C++LIBS}/msvc/freeglut/freeglut-3.0.0/lib/x64/libfreeglut.lib")
            set(GLUT_INCLUDE_DIR "${C++LIBS}/msvc/freeglut/freeglut-3.0.0/include")
        else ()
            # GLUT 32
            set(GLUT_glut_LIBRARY "{C++LIBS}/msvc/freeglut/freeglut-3.0.0/lib/libfreeglut.lib")
            set(GLUT_INCLUDE_DIR "${C++LIBS}/msvc/freeglut/freeglut-3.0.0/include")
        endif ()
    endif ()

    # Set path for GLM
    set(GLM_INCLUDE_DIR "${C++LIBS}/all/glm/glm-0.9.9.0")
else()
    message(STATUS "Platform: Linux")
endif ()

message("\n#####################################")

### END SETUP FOR WINDOWS ##############################################################################################
########################################################################################################################
### FIND OPENGL ########################################################################################################

find_package(OpenGL REQUIRED)

message("")
message(STATUS "OPENGL_FOUND: ${OPENGL_FOUND}")
message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message(STATUS "OPENGL_INCLUDE_DIRS: ${OPENGL_INCLUDE_DIRS}")

### END OPENGL #########################################################################################################
########################################################################################################################
### FIND GLUT ##########################################################################################################

find_package(GLUT REQUIRED)

message("")
message(STATUS "GLUT_FOUND: ${GLUT_FOUND}")
message(STATUS "GLUT_glut_LIBRARY: ${GLUT_glut_LIBRARY}")
message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")

### END GLUT ###########################################################################################################
########################################################################################################################
### CHECK GLM ##########################################################################################################

message("")
message(STATUS "GLM_INCLUDE_DIR: ${GLM_INCLUDE_DIR}")
message("")

### END GLM ############################################################################################################
########################################################################################################################
### LINK AND BUILD #####################################################################################################

include_directories(${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
add_executable(gm_exercise src/gmuebung.cpp src/BezierCurve.cpp src/BezierCurve.h src/AxisAlignedBoundingBox.cpp src/AxisAlignedBoundingBox.h src/Line.h src/Util.h src/DrawUtil.h)
target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

### END LINK AND BUILD #################################################################################################
########################################################################################################################