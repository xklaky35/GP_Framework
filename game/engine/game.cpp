#include "game.h"
#include "renderer.h"
#include "logmanager/logmanager.h"
#include "../config/config.h"
#include "imgui.h"

#include <SDL_timer.h>
#include <SDL_ttf.h>

#include "../scenes/bouncingball/scenebouncingball.h"
#include "../scenes/slashscreen/splashscreen.h"
#include "../scenes/spaceinvader/scenespaceinvader.h"
#include "../scenes/mainmenu/mainmenu.h"
#include "imgui/imguimanager.h"
#include "input/input.h"
#include "scenemanager/scenemanager.h"

#include "fmod.hpp"
#include "fmod_errors.h"
#include "sound/soundmanager.h"

#define DEBUG

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
        SoundManager::DestroyInstance();
        InputManager::DestroyInstance();
        SceneManager::DestroyInstance();


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
                   m_bIsPaused(false),
                   m_bIsDebugView(false) {
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

        if (!SoundManager::GetInstance().Initialise()) {
            return false;
        }
        SoundManager::GetInstance().LoadAudio();

        if (!ImguiManager::GetInstance().Initialise(m_pRenderer->GetSDLWindow(), m_pRenderer->GetSDLGLContext())) {
            return false;
        }

        //################ INIT STUFF HERE ####################

        SceneManager::GetInstance().RegisterScene("Splash", new Splashscreen());
        SceneManager::GetInstance().RegisterScene("MainMenu", new MainMenu());
        SceneManager::GetInstance().RegisterScene("SpaceInvaders", new SceneSpaceinvader());
        SceneManager::GetInstance().RegisterScene("Robotron", new SceneBouncingBall());
        SceneManager::GetInstance().RegisterScene("Astroids", new SceneBouncingBall());
        SceneManager::GetInstance().LoadScene("Splash");

        //################ INIT STUFF HERE ####################

        bbWidth = m_pRenderer->GetWidth();
        bbHeight = m_pRenderer->GetHeight();
        m_iLastTime = SDL_GetPerformanceCounter();
        m_pRenderer->SetClearColour(0, 255, 255);



        return true;
    }

    bool Game::DoGameLoop() {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {

            ImguiManager::GetInstance().ProcessEvent(event);
            InputManager::GetCurrentEvents().RegisterEvent(event);
        }

        if (m_bLooping) {

            const Uint64 current = SDL_GetPerformanceCounter();
            float deltaTime = static_cast<float>(current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
            m_iLastTime = current;
            m_fExecutionTime += deltaTime;

            Process(deltaTime);
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
            SoundManager::GetInstance().Process(deltaTime);

            // ###########################################
        }
        SceneManager::GetInstance().GetCurrentScene()->SystemProcess();
    }



    void Game::Draw(Renderer &renderer) {
        ++m_iFrameCount;
        renderer.Clear();

        // ####### RENDER STUFF HERE #############

        SceneManager::GetInstance().GetCurrentScene()->Draw(renderer);
        renderer.Draw();

        // #######################################

#ifdef DEBUG
        if (m_bIsDebugView) {
            DrawDebug(&m_bIsDebugView);
            SceneManager::GetInstance().DrawDebug(&m_bIsDebugView);
        }
#endif

        ImguiManager::GetInstance().Draw();
        renderer.Present();
    }

    void Game::DrawDebug(bool* p_open) {
        ImGui::BeginMainMenuBar();

        if (ImGui::ArrowButton(m_bIsPaused ? "Start" : "Stop", ImGuiDir_Right)) {
            TogglePause();
        }

        if (ImGui::ColorButton("X", ImVec4(255,0,0,1))) {
            Quit();
        }

        if (ImGui::ColorButton("Reset", ImVec4(0,255,255,1))) {
            SceneManager::GetInstance().ResetCurrentScene();
        }

        ImGui::EndMainMenuBar();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        const float PAD = 30.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x =  (work_pos.x + work_size.x - PAD);
        window_pos.y =  (work_pos.y + PAD);
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 1.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Stats", 0, window_flags))
        {
            ImGui::Text("FPS: (%.1f)", static_cast<double>(m_iFPS));
        }
        ImGui::End();
    }

    void Game::ProcessFrameCounting(float deltaTime) {
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

    bool Game::IsDebug() const {
        return m_bIsDebugView;
    }
    bool Game::IsPaused() const {
        return m_bIsPaused;
    }

    void Game::TogglePause() {
        m_bIsPaused = !m_bIsPaused;
    }
}
