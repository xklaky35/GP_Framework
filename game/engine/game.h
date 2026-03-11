// COMP710 GP Framework
#ifndef GAME_H
#define GAME_H

#include <SDL_stdinc.h>

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

    protected:
        void Process(float deltaTime);
        void Draw(Renderer &renderer);
        void ProcessFrameCounting(float deltaTime);

    private:
        Game();
        ~Game();

    protected:
        static Game *sm_pInstance;
        Renderer *m_pRenderer;
        Uint64 m_iLastTime;
        float m_fExecutionTime;
        float m_fElapsedSeconds;
        int m_iFrameCount;
        int m_iFPS;
#ifdef USE_LAG
        float m_fLag;
        int m_iUpdateCount;
#endif // USE_LAG
        bool m_bLooping;

    private:
    };
}
#endif // GAME_H
