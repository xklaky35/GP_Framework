cmake_minimum_required(VERSION 4.1)
project(GP_Framework)

set(CMAKE_CXX_STANDARD 20)

add_executable(GP_Framework WIN32 main.cpp)

# this assumes the SDL source is available in lib/SDL2
add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
# this assumes the SDL source is available in lib/SDL2_image
add_subdirectory(lib/SDL2_image EXCLUDE_FROM_ALL)

# glew include setup
find_library(GLEWLIB GLEW lib/GLEW/lib)
include_directories(lib/GLEW/include)

# dependency for glew
find_package(OpenGL REQUIRED)

target_link_libraries(
        GP_Framework
        PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        ${GLEWLIB}
        OpenGL
)