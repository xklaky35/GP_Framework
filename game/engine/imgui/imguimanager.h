#ifndef GP_FRAMEWORK_IMGUIMANAGER_H
#define GP_FRAMEWORK_IMGUIMANAGER_H

#define IS_HELP_SHOWN false

#include <SDL_video.h>
#include <SDL.h>

#include "../game.h"
#include "../scenemanager/scenemanager.h"


namespace Engine {

    struct UiData {
        bool ShowMainMenuBar = true;
        bool ShowSceneManager = true;
    };

    class ImguiManager {
    public:

        static ImguiManager& GetInstance();
        static void DestroyInstance();
        static void HelpMarker(const char*);


        bool Initialise(SDL_Window*, SDL_GLContext);
        void ProcessEvent(SDL_Event);
        void Process();
        void Draw();
        void DrawDebugSceneManager(SceneManager*);

    public:
        bool m_bShowDemoWindow;
        UiData m_UiData;
    private:
        static ImguiManager* m_pInstance;
        ImguiManager();
        ~ImguiManager();
    };

}

#endif //GP_FRAMEWORK_IMGUIMANAGER_H