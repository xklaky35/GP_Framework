#ifndef GP_FRAMEWORK_IMGUIMANAGER_H
#define GP_FRAMEWORK_IMGUIMANAGER_H

#include <SDL_video.h>
#include <SDL.h>


namespace Engine {
    class ImguiManager {
    public:

        static ImguiManager& GetInstance();
        static void DestroyInstance();


        void Initialize(SDL_Window*, SDL_GLContext);
        void ProcessEvent(SDL_Event);
        void Process();
        void Draw();

    public:
        bool m_bShowDemoWindow;
    private:
        static ImguiManager* m_pInstance;
        ImguiManager();
        ~ImguiManager();
    };

}

#endif //GP_FRAMEWORK_IMGUIMANAGER_H