# required
cmake_minimum_required(VERSION 3.10)

# set some flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

# set the project name and version
project(NSNES VERSION 0.0)

# add a configuration header
configure_file("${PROJECT_SRC_DIR}NSNES_config.h.in" "${PROJECT_SRC_DIR}generated/NSNES_config.h")

# get source files
file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
file(GLOB_RECURSE INCLUDES src/*.hpp src/*.h)

# add the executable
add_executable(NSNES ${SRCS} ${INCLUDES})

# add the build directory as an include directory (for the config header)
target_include_directories(NSNES PUBLIC
                            "${PROJECT_SRC_DIR}generated"
                          )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(NSNES PUBLIC
                       ${OPENGL_gl_LIBRARY}
                       ${GLEW_LIBRARIES}
                       -lSDL2
                       -lSDL2_image
                       -lSDL2_ttf
                       -lopenal
                       -lsndfile
                       -pthread
                     )
