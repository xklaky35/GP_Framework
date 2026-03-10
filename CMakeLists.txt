cmake_minimum_required(VERSION 4.1)
project(GP_Framework)

set(CMAKE_CXX_STANDARD 20)

add_executable(GP_Framework WIN32 game/main.cpp
        game/logmanager/logmanager.cpp
        game/logmanager/logmanager.h
        game/game/game.cpp
        game/game/game.h
        game/renderer/renderer.cpp
        game/renderer/renderer.h
        game/texture/texture.cpp
        game/texture/texture.h
        game/texture/texturemanager.cpp
        game/texture/texturemanager.h
        game/shader/shader.cpp
        game/shader/shader.h
        game/matrix4/matrix4.cpp
        game/matrix4/matrix4.h
        game/vertexarray/vertexarray.cpp
        game/vertexarray/vertexarray.h
        game/sprite/sprite.cpp
        game/sprite/sprite.h)

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