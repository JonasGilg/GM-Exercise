cmake_minimum_required(VERSION 3.10)

project(GM-Exercise)
set(CMAKE_CXX_STANDARD 17)

###############################################
### SETUP FOR WINDOWS #########################

if (WIN32)
    message("Using WIN32")

    # Check if 64bit is enabled
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(64BIT "TRUE")
        message("64bit")
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(64BIT "FALSE")
        message("32bit")
    endif ()

    # Get Path to C++ library directory and store ot in "C++LIBS" variable
    file(TO_CMAKE_PATH $ENV{CPP_LIBS} C++LIBS)

    # Check toolchain, 32/64bit and set variables accordingly
    if (MINGW)
        message("Using MinGW")

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
        message("Using MSVC")

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
endif ()

### END SETUP FOR WINDOWS ##############################################################################################
########################################################################################################################
### FIND OPENGL ########################################################################################################

find_package(OpenGL REQUIRED)

message("")
message("OPENGL_FOUND: ${OPENGL_FOUND}")
message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message("OPENGL_INCLUDE_DIRS: ${OPENGL_INCLUDE_DIRS}")

### END OPENGL #########################################################################################################
########################################################################################################################
### FIND GLUT ##########################################################################################################

find_package(GLUT REQUIRED)

message("")
message("GLUT_FOUND: ${GLUT_FOUND}")
message("GLUT_glut_LIBRARY: ${GLUT_glut_LIBRARY}")
message("GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")

### END GLUT ###########################################################################################################
########################################################################################################################
### CHECK GLM ##########################################################################################################

message("")
message("GLM_INCLUDE_DIR: ${GLM_INCLUDE_DIR}")
message("")

### END GLM ############################################################################################################
########################################################################################################################
### LINK AND BUILD #####################################################################################################

include_directories(${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
add_executable(gm_exercise gmuebung.cpp BezierCurve.cpp BezierCurve.h AxisAlignedBoundingBox.cpp AxisAlignedBoundingBox.h)
target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

### END LINK AND BUILD #################################################################################################
########################################################################################################################