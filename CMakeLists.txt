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
    set(SDL_PATH "SDL2/SDL2-2.0.8/i686-w64-mingw32")

    # Check toolchain, 32/64bit and set variables accordingly
    if (MINGW)
        message(STATUS "Toolchain: MinGW")

        if (64BIT)
            # SDL
            set(SDL_LIBRARY "${C++LIBS}/mingw/${SDL_PATH}/lib")
            set(SDL_INCLUDE_DIR "${C++LIBS}/mingw/${SDL_PATH}/include")
        else()
            set(SDL_LIBRARY "${C++LIBS}/mingw/${SDL_PATH}/lib")
            set(SDL_INCLUDE_DIR "${C++LIBS}/mingw/${SDL_PATH}/include")
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
### FIND SDL ##########################################################################################################


message("")
message(STATUS "SDL_LIBRARY: ${SDL_LIBRARY}")
message(STATUS "SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")

### END GLUT ###########################################################################################################
########################################################################################################################
### CHECK GLM ##########################################################################################################

message("")
message(STATUS "GLM_INCLUDE_DIR: ${GLM_INCLUDE_DIR}")
message("")

### END GLM ############################################################################################################
########################################################################################################################
### LINK AND BUILD #####################################################################################################

include_directories(${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${SDL_INCLUDE_DIR})
add_executable(gm_exercise src/gmuebung.cpp src/BezierCurve.cpp src/BezierCurve.h src/AxisAlignedBoundingBox.cpp src/AxisAlignedBoundingBox.h src/Line.h src/Util.h src/DrawUtil.h src/Window.cpp src/Window.h)
target_link_libraries(gm_exercise ${OPENGL_LIBRARIES} ${SDL_LIBRARY})

### END LINK AND BUILD #################################################################################################
########################################################################################################################