cmake_minimum_required(VERSION 4.1)
project(GP_Framework)

set(CMAKE_CXX_STANDARD 20)

############### ENGINE ###################

set(ENGINE_DIR game/engine)
set(ENGINE_NODES ${ENGINE_DIR}/nodes)
set(ENGINE_SOURCES
        ${ENGINE_DIR}/game.cpp
        ${ENGINE_DIR}/game.h
        ${ENGINE_DIR}/logmanager/logmanager.cpp
        ${ENGINE_DIR}/logmanager/logmanager.h
        ${ENGINE_DIR}/renderer.cpp
        ${ENGINE_DIR}/renderer.h
        ${ENGINE_DIR}/texture.cpp
        ${ENGINE_DIR}/texture.h
        ${ENGINE_DIR}/texturemanager.cpp
        ${ENGINE_DIR}/texturemanager.h
        ${ENGINE_DIR}/shader.cpp
        ${ENGINE_DIR}/shader.h
        ${ENGINE_DIR}/structs/matrix4.cpp
        ${ENGINE_DIR}/structs/matrix4.h
        ${ENGINE_DIR}/structs/vertexarray.cpp
        ${ENGINE_DIR}/structs/vertexarray.h
        ${ENGINE_DIR}/sprite.cpp
        ${ENGINE_DIR}/sprite.h
        ${ENGINE_DIR}/structs/vector2d.cpp
        ${ENGINE_DIR}/structs/vector2d.h
        ${ENGINE_DIR}/structs/transform.cpp
        ${ENGINE_DIR}/structs/transform.h
        ${ENGINE_NODES}/node.cpp
        ${ENGINE_NODES}/node.h
        ${ENGINE_NODES}/spritenode.cpp
        ${ENGINE_NODES}/spritenode.h
        ${ENGINE_NODES}/collidernode.cpp
        ${ENGINE_NODES}/collidernode.h
        ${ENGINE_NODES}/forms/form.h
        ${ENGINE_NODES}/collisionmanager.cpp
        ${ENGINE_NODES}/collisionmanager.h
        ${ENGINE_NODES}/forms/circle.cpp
        ${ENGINE_NODES}/forms/circle.h
        ${ENGINE_NODES}/forms/rectangle.cpp
        ${ENGINE_NODES}/forms/rectangle.h
        ${ENGINE_NODES}/forms/form.cpp
        ${ENGINE_NODES}/ridgitbody.cpp
        ${ENGINE_NODES}/ridgitbody.h
        ${ENGINE_DIR}/input/input.cpp
        ${ENGINE_DIR}/input/input.h
        ${ENGINE_DIR}/imgui/imguimanager.cpp
        ${ENGINE_DIR}/imgui/imguimanager.h
        game/helper/inlinehelper.h

)

################## IMGUI ####################

# Define paths to ImGui sources and backends
set(IMGUI_DIR lib/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp
        game/scenes/slashscreen/splashscreen.cpp
        game/scenes/slashscreen/splashscreen.h
        game/engine/scenemanager/scenemanager.cpp
        game/engine/scenemanager/scenemanager.h
        game/scenes/bouncingball/scenebouncingball.cpp
        game/scenes/bouncingball/scenebouncingball.h
        game/engine/nodes/animatedspritenode.cpp
        game/engine/nodes/animatedspritenode.h
        game/engine/event.h
)




add_executable(GP_Framework
        game/main.cpp
        game/config/config.cpp
        game/config/config.h
        game/scenes/bouncingball/bouncingball.cpp
        game/scenes/bouncingball/bouncingball.h
        game/scenes/spaceinvader/spaceship.cpp
        game/scenes/spaceinvader/spaceship.h
        game/scenes/spaceinvader/meteors.cpp
        game/scenes/spaceinvader/meteors.h
        game/scenes/spaceinvader/meteorgenerator.cpp
        game/scenes/spaceinvader/meteorgenerator.h
        ${IMGUI_SOURCES}
        ${ENGINE_SOURCES}
        game/scenes/spaceinvader/scenespaceinvader.cpp
        game/scenes/spaceinvader/scenespaceinvader.h
)


########## SDL 2 ##############
# this assumes the SDL source is available in lib/SDL2
add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
# this assumes the SDL source is available in lib/SDL2_image
add_subdirectory(lib/SDL2_image EXCLUDE_FROM_ALL)



############ GLEW ###############
# glew include setup
find_library(GLEWLIB GLEW lib/GLEW/lib)
target_include_directories(GP_Framework PRIVATE lib/GLEW/include ${IMGUI_DIR} ${IMGUI_BACKENDS_DIR})

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