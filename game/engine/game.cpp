#include "game.h"
#include "renderer.h"
#include "logmanager/logmanager.h"
#include "nodetree/nodetree.h"
#include "../config/config.h"

#include <SDL_timer.h>

#include "nodetree/collisionmanager.h"


namespace Engine {
    // Static Members:
    Game *Game::sm_pInstance = nullptr;

    Game &Game::GetInstance() {
        if (sm_pInstance == nullptr) {
            sm_pInstance = new Game();
        }
        return (*sm_pInstance);
    }

    void Game::DestroyInstance() {
        delete sm_pInstance;
        sm_pInstance = nullptr;
    }

    Game::Game() : m_pRenderer(nullptr),
                   m_iLastTime(0),
                   m_fExecutionTime(0),
                   m_fElapsedSeconds(0),
                   m_iFrameCount(0),
                   m_iFPS(0),
                   m_bLooping(true) {
    }

    Game::~Game() {
        delete m_pRenderer;
        m_pRenderer = nullptr;
    }


    void Game::Quit() {
        m_bLooping = false;
    }

    bool Game::Initialise() {
        Config::GetInstance().SetDefaultConfig();

        int bbWidth = Config::GetInstance().windowsWidth;
        int bbHeight = Config::GetInstance().windowsHeight;
        m_pRenderer = new Renderer();
        if (!m_pRenderer->Initialise(true, bbWidth, bbHeight)) {
            LogManager::GetInstance().Log(ERROR, "Renderer failed to initialise!");
            return false;
        }

        Nodetree::GetInstance().Init();

        bbWidth = m_pRenderer->GetWidth();
        bbHeight = m_pRenderer->GetHeight();
        m_iLastTime = SDL_GetPerformanceCounter();
        m_pRenderer->SetClearColour(0, 255, 255);
        return true;
    }

    bool Game::DoGameLoop() {
        const float stepSize = 1.0f / 60.0f;

        if (m_bLooping) {
            const Uint64 current = SDL_GetPerformanceCounter();
            float deltaTime = static_cast<float>(current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
            m_iLastTime = current;
            m_fExecutionTime += deltaTime;
            Process(deltaTime);
#ifdef USE_LAG
            m_fLag += deltaTime;
            int innerLag = 0;
            while (m_fLag >= stepSize) {
                Process(stepSize);
                m_fLag -= stepSize;
                ++m_iUpdateCount;
                ++innerLag;
            }
#endif //USE_LAG
            Draw(*m_pRenderer);
        }
        return m_bLooping;
    }

    void Game::Process(const float deltaTime) {
        ProcessFrameCounting(deltaTime);

        Nodetree::GetInstance().Process(deltaTime);
    }


    void Game::Draw(Renderer &renderer) {
        ++m_iFrameCount;
        renderer.Clear();
        // TODO: Add game objects to draw here!

        Nodetree::GetInstance().Draw(renderer);

        /*
        Sprite* board_t_left = renderer.CreateSprite("../assets/Sprites/board8x8.png");
        Sprite* board_center = renderer.CreateSprite("../assets/Sprites/board8x8.png");
        Sprite* board_b_left = renderer.CreateSprite("../assets/Sprites/board8x8.png");
        Sprite* board_b_right = renderer.CreateSprite("../assets/Sprites/board8x8.png");
        Sprite* board_t_right = renderer.CreateSprite("../assets/Sprites/board8x8.png");



        // top right
        board_t_right->SetX(renderer.GetWidth() - board_t_right->GetWidth() / 2);
        board_t_right->SetY(board_t_right->GetHeight() / 2);
        board_t_right->SetBlueTint(0);
        board_t_right->SetRedTint(1);
        board_t_right->SetGreenTint(0);


        // top left
        board_t_left->SetX(board_t_left->GetWidth() / 2);
        board_t_left->SetY(board_t_left->GetHeight() / 2);


        // bottom right
        board_b_right->SetX(renderer.GetWidth()-board_b_right->GetWidth() / 2);
        board_b_right->SetY(renderer.GetHeight()-board_b_right->GetHeight() / 2);
        board_b_right->SetBlueTint(0);
        board_b_right->SetRedTint(0);
        board_b_right->SetGreenTint(1);

        // bottom left
        board_b_left->SetX(board_t_left->GetWidth() / 2);
        board_b_left->SetY(renderer.GetHeight() - board_t_left->GetHeight() / 2);
        board_b_left->SetBlueTint(1);
        board_b_left->SetRedTint(0);
        board_b_left->SetGreenTint(0);

        // center
        board_center->SetX(renderer.GetWidth() / 2);
        board_center->SetY(renderer.GetHeight() / 2);
        board_center->SetAngle(45);
        board_center->SetScale(2);


        // draw them
        renderer.DrawSprite(*board_t_left);
        renderer.DrawSprite(*board_t_right);
        renderer.DrawSprite(*board_b_left);
        renderer.DrawSprite(*board_b_right);
        renderer.DrawSprite(*board_center);
        */

        renderer.Present();
    }

    void
    Game::ProcessFrameCounting(float deltaTime) {
        // Count total simulation time elapsed:
        m_fElapsedSeconds += deltaTime;
        // Frame Counter:
        if (m_fElapsedSeconds > 1.0f) {
            m_fElapsedSeconds -= 1.0f;
            m_iFPS = m_iFrameCount;
            m_iFrameCount = 0;
        }
    }
}
