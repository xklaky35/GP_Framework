cmake_minimum_required(VERSION 4.1)
project(GP_Framework)

set(CMAKE_CXX_STANDARD 20)

############### SCENES ###################

set(SCENES_DIR game/scenes)
set(SCENES_SOURCES
        ${SCENES_DIR}/bouncingball/scenebouncingball.cpp
        ${SCENES_DIR}/slashscreen/splashscreen.cpp
        ${SCENES_DIR}/slashscreen/splashscreen.h
        ${SCENES_DIR}/bouncingball/scenebouncingball.h
        ${SCENES_DIR}/mainmenu/mainmenu.cpp
        ${SCENES_DIR}/mainmenu/mainmenu.h
        ${SCENES_DIR}/bouncingball/bouncingball.cpp
        ${SCENES_DIR}/bouncingball/bouncingball.h
        ${SCENES_DIR}/spaceinvader/spaceship.cpp
        ${SCENES_DIR}/spaceinvader/spaceship.h
        ${SCENES_DIR}/spaceinvader/meteors.cpp
        ${SCENES_DIR}/spaceinvader/meteors.h
        ${SCENES_DIR}/spaceinvader/meteorgenerator.cpp
        ${SCENES_DIR}/spaceinvader/meteorgenerator.h
        ${SCENES_DIR}/spaceinvader/scenespaceinvader.cpp
        ${SCENES_DIR}/spaceinvader/scenespaceinvader.h
        ${SCENES_DIR}/spaceinvader/bullet.cpp
        ${SCENES_DIR}/spaceinvader/bullet.h
        ${SCENES_DIR}/whoosh/scenewhoosh.cpp
        ${SCENES_DIR}/whoosh/scenewhoosh.h
        ${SCENES_DIR}/spaceinvader/bulletspawner.cpp
        ${SCENES_DIR}/spaceinvader/bulletspawner.h
        ${SCENES_DIR}/whoosh/player.cpp
        ${SCENES_DIR}/whoosh/player.h
        ${SCENES_DIR}/whoosh/Level1/Level1.cpp
        ${SCENES_DIR}/whoosh/Level1/Level1.h
        ${SCENES_DIR}/whoosh/Level1/colliderlayerlevel1.cpp
        ${SCENES_DIR}/whoosh/Level1/colliderlayerlevel1.h
        ${SCENES_DIR}/whoosh/Level1/spritelayerlevel1.cpp
        ${SCENES_DIR}/whoosh/Level1/spritelayerlevel1.h
        ${SCENES_DIR}/whoosh/levelgoal.cpp
        ${SCENES_DIR}/whoosh/levelgoal.h
        ${SCENES_DIR}/winscreen/winscreen.cpp
        ${SCENES_DIR}/winscreen/winscreen.h
        ${SCENES_DIR}/whoosh/Level1/foregroundlayerlevel1.cpp
        ${SCENES_DIR}/whoosh/Level1/foregroundlayerlevel1.h
        ${SCENES_DIR}/whoosh/enemy.cpp
        ${SCENES_DIR}/whoosh/enemy.h
)


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
        ${ENGINE_DIR}/input/input.cpp
        ${ENGINE_DIR}/input/input.h
        ${ENGINE_DIR}/imgui/imguimanager.cpp
        ${ENGINE_DIR}/imgui/imguimanager.h
        ${ENGINE_DIR}/scenemanager/scenemanager.cpp
        ${ENGINE_DIR}/scenemanager/scenemanager.h
        ${ENGINE_DIR}/event.h
        ${ENGINE_DIR}/sound/soundmanager.cpp
        ${ENGINE_DIR}/sound/soundmanager.h
        ${ENGINE_DIR}/physics/physicsmanager.cpp
        ${ENGINE_DIR}/physics/physicsmanager.h
        ${ENGINE_DIR}/nodes/nodefactory.cpp
        ${ENGINE_DIR}/nodes/nodefactory.h
        ${ENGINE_DIR}/assetmanager/assetmanager.cpp
        ${ENGINE_DIR}/assetmanager/assetmanager.h
        ${ENGINE_DIR}/assetmanager/assetbrowser.cpp
        ${ENGINE_DIR}/assetmanager/assetbrowser.h
        ${ENGINE_DIR}/imgui/imguiwindowbaseclass.h
        ${ENGINE_DIR}/iniparser/iniparser.cpp
        ${ENGINE_DIR}/iniparser/iniparser.h
        ${ENGINE_DIR}/animatedsprite.cpp
        ${ENGINE_DIR}/animatedsprite.h
        ${ENGINE_DIR}/scenemanager/sceneselector.cpp
        ${ENGINE_DIR}/scenemanager/sceneselector.h
        ${ENGINE_DIR}/physics/debugdraw.cpp
        ${ENGINE_DIR}/physics/debugdraw.h
        ${ENGINE_DIR}/nodes/cameranode.cpp
        ${ENGINE_DIR}/nodes/cameranode.h
        ${ENGINE_DIR}/structs/vector3d.cpp
        ${ENGINE_DIR}/structs/vector3d.h
        ${ENGINE_DIR}/physics/DebugFormsCreator.cpp
        ${ENGINE_DIR}/physics/DebugFormsCreator.h
        ${ENGINE_DIR}/time/timer.cpp
        ${ENGINE_DIR}/time/timer.h
        ${ENGINE_DIR}/nodes/ui/button.cpp
        ${ENGINE_DIR}/nodes/ui/button.h


        ${ENGINE_NODES}/node.cpp
        ${ENGINE_NODES}/node.h
        ${ENGINE_NODES}/spritenode.cpp
        ${ENGINE_NODES}/spritenode.h
        ${ENGINE_NODES}/collidernode.cpp
        ${ENGINE_NODES}/collidernode.h
        ${ENGINE_NODES}/animatedspritenode.cpp
        ${ENGINE_NODES}/animatedspritenode.h
        ${ENGINE_NODES}/ui/control.cpp
        ${ENGINE_NODES}/ui/control.h
        ${ENGINE_NODES}/ui/container.cpp
        ${ENGINE_NODES}/ui/container.h
        ${ENGINE_NODES}/ui/vcontainer.cpp
        ${ENGINE_NODES}/ui/vcontainer.h
        ${ENGINE_NODES}/ui/hcontainer.cpp
        ${ENGINE_NODES}/ui/hcontainer.h
        ${ENGINE_NODES}/ui/margincontainer.cpp
        ${ENGINE_NODES}/ui/margincontainer.h
        ${ENGINE_NODES}/ui/panelcontainer.cpp
        ${ENGINE_NODES}/ui/panelcontainer.h
        ${ENGINE_NODES}/ui/textcontrol.cpp
        ${ENGINE_NODES}/ui/textcontrol.h
        ${ENGINE_NODES}/rigidbodynode.cpp
        ${ENGINE_NODES}/rigidbodynode.h
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
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_executable(GP_Framework
        game/main.cpp
        game/config/config.cpp
        game/config/config.h
        ${IMGUI_SOURCES}
        ${ENGINE_SOURCES}
        ${SCENES_SOURCES}
)



########## FMOD ##############

set(FMOD_DIR lib/FMOD/api)
set(FMOD_CORE_API ${FMOD_DIR}/core/inc)
set(FMOD_CORE_LIB ${FMOD_DIR}/core/lib/x86_64)
set(FMOD_FSBANK_API ${FMOD_DIR}/fsbank/inc)
set(FMOD_FSBANK_LIB ${FMOD_DIR}/fsbank/lib/x86_64)
set(FMOD_STUDIO_API ${FMOD_DIR}/studio/inc)
set(FMOD_STUDIO_LIB ${FMOD_DIR}/studio/lib/x86_64)

# setup static library directories
find_library(FMODCORELIB NAMES fmod libfmod.so.14 ${FMOD_CORE_LIB})
find_library(FMODSTUDIOLIB NAMES fmodstudio libfmodstudio.so.14 ${FMOD_STUDIO_LIB})
find_library(FMODFSBANKLIB NAMES fsbank libfsbank.so.14 ${FMOD_FSBANK_LIB})



########## SDL 2 ##############
# this assumes the SDL source is available in lib/SDL2
add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
# this assumes the SDL source is available in lib/SDL2_image
add_subdirectory(lib/SDL2_image EXCLUDE_FROM_ALL)

add_subdirectory(lib/SDL2_ttf EXCLUDE_FROM_ALL)

add_subdirectory(lib/BOX2D EXCLUDE_FROM_ALL)



############ GLEW ###############
# glew include setup
find_library(GLEWLIB GLEW lib/GLEW/lib)

# dependency for glew
find_package(OpenGL REQUIRED)



target_include_directories(GP_Framework PRIVATE
        lib/GLEW/include
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS_DIR}
        ${FMOD_CORE_API}
        ${FMOD_FSBANK_API}
        ${FMOD_STUDIO_API})


target_link_libraries(
        GP_Framework
        PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        box2d::box2d
        ${GLEWLIB}
        ${FMODCORELIB}
        ${FMODSTUDIOLIB}
        ${FMODFSBANKLIB}
        OpenGL
        GL
        GLU
        glut
)

add_definitions(-DPROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")
