// COMP710 GP Framework
#ifndef GAME_H
#define GAME_H

#include <SDL_stdinc.h>
#include <vector>

#include "nodes/node.h"

namespace Engine {
    // Forward declarations:
    class Renderer;
    class Nodetree;

    class Game {
        // Member methods:
    public:
        static Game &GetInstance();
        static void DestroyInstance();
        bool Initialise();
        bool DoGameLoop();
        void Quit();
        void ToggleViewDebug();

        bool IsDebug() const;
        bool IsPaused() const;

        void TogglePause();

    protected:
        void Process(float deltaTime);
        void Draw(Renderer &renderer);
        void DrawDebug(bool*);
        void ProcessFrameCounting(float deltaTime);

    private:
        Game();
        ~Game();

    protected:
        static Game *sm_pInstance;
        Renderer *m_pRenderer;
        Uint64 m_iLastTime;
        float m_fExecutionTime;
        float m_iFPS;
        float m_fElapsedSeconds;
        int m_iFrameCount;

        bool m_bLooping;
        bool m_bIsPaused;
        bool m_bIsDebugView;

    };
}
#endif // GAME_H
