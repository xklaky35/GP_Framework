#include "game.h"
#include "renderer.h"
#include "logmanager/logmanager.h"
#include "../config/config.h"

#include <SDL_timer.h>

#include "nodes/collisionmanager.h"

#include "imgui.h"
#include "../scenes/slashscreen/splashscreen.h"
#include "../scenes/spaceinvader/metheorgenerator.h"
#include "imgui/imguimanager.h"
#include "input/input.h"
#include "scenemanager/scenemanager.h"

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

        LogManager::DestroyInstance();
        ImguiManager::DestroyInstance();

        delete sm_pInstance;
        sm_pInstance = nullptr;
    }

    Game::Game() : m_pRenderer(nullptr),
                   m_iLastTime(0),
                   m_fExecutionTime(0),
                   m_fElapsedSeconds(0),
                   m_iFrameCount(0),
                   m_iFPS(0),
                   m_bLooping(true),
                   m_bIsPaused(false) {
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


        //################ INIT STUFF HERE ####################

        ImguiManager::GetInstance().Initialize(m_pRenderer->GetSDLWindow(), m_pRenderer->GetSDLGLContext());

        SceneManager::GetInstance().RegisterScene("Splash", new Splashscreen());
        SceneManager::GetInstance().RegisterScene("Game", new MetheorGenerator());
        SceneManager::GetInstance().LoadScene("Splash");

        //################ INIT STUFF HERE ####################



        bbWidth = m_pRenderer->GetWidth();
        bbHeight = m_pRenderer->GetHeight();
        m_iLastTime = SDL_GetPerformanceCounter();
        m_pRenderer->SetClearColour(0, 255, 255);


        return true;
    }

    bool Game::DoGameLoop() {
        const float stepSize = 1.0f / 60.0f;

        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {

            ImguiManager::GetInstance().ProcessEvent(event);


            Input::GetCurrentEvents().RegisterEvent(event);
        }

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
        ImguiManager::GetInstance().Process();
        if (ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
            ToggleViewDebug();
        }

        if (!m_bIsPaused) {
            // ####### MAKE LOGIC STUFF HERE #############

            SceneManager::GetInstance().GetCurrentScene()->Process(deltaTime);

            // ###########################################
        }

    }


    void Game::Draw(Renderer &renderer) {
        ++m_iFrameCount;
        renderer.Clear();

        // ####### RENDER STUFF HERE #############

        SceneManager::GetInstance().GetCurrentScene()->Draw(renderer);

        // #######################################

        DrawDebug();
        ImguiManager::GetInstance().Draw();
        renderer.Present();
    }

    void Game::DrawDebug()
    {
        if (m_bIsDebugView) {
            ImGui::BeginMainMenuBar();

            if (ImGui::ArrowButton(m_bIsPaused?"Start":"Stop", ImGuiDir_Right))
            {
                m_bIsPaused = !m_bIsPaused;
            }

            if (ImGui::Button("Quit"))
            {
                Quit();
            }

            ImGui::EndMainMenuBar();
            SceneManager::GetInstance().DrawDebug();
        }

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

    void Game::ToggleViewDebug() {
        m_bIsDebugView = !m_bIsDebugView;
    }
}
