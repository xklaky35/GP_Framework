cmake_minimum_required(VERSION 4.1)
project(GP_Framework)

set(CMAKE_CXX_STANDARD 20)

add_executable(GP_Framework WIN32 game/main.cpp
        game/engine/logmanager/logmanager.cpp
        game/engine/logmanager/logmanager.h
        game/engine/game.cpp
        game/engine/game.h
        game/engine/renderer.cpp
        game/engine/renderer.h
        game/engine/texture.cpp
        game/engine/texture.h
        game/engine/texturemanager.cpp
        game/engine/texturemanager.h
        game/engine/shader.cpp
        game/engine/shader.h
        game/engine/structs/matrix4.cpp
        game/engine/structs/matrix4.h
        game/engine/structs/vertexarray.cpp
        game/engine/structs/vertexarray.h
        game/engine/sprite.cpp
        game/engine/sprite.h
        game/engine/nodetree/node.cpp
        game/engine/nodetree/node.h
        game/engine/nodetree/nodetree.cpp
        game/engine/nodetree/nodetree.h
        game/engine/structs/vector2d.cpp
        game/engine/structs/vector2d.h
        game/engine/nodetree/spritenode.cpp
        game/engine/nodetree/spritenode.h
        game/entities/spaceship.cpp
        game/entities/spaceship.h
        game/engine/structs/transform.cpp
        game/engine/structs/transform.h
        game/config/config.cpp
        game/config/config.h
        game/entities/meteors.cpp
        game/entities/meteors.h
        game/entities/metheorgenerator.cpp
        game/entities/metheorgenerator.h
        game/engine/nodetree/collidernode.cpp
        game/engine/nodetree/collidernode.h
        game/engine/nodetree/forms/form.h
        game/engine/nodetree/collisionmanager.cpp
        game/engine/nodetree/collisionmanager.h
        game/engine/nodetree/forms/circle.cpp
        game/engine/nodetree/forms/circle.h
        game/engine/nodetree/forms/rectangle.cpp
        game/engine/nodetree/forms/rectangle.h
)

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