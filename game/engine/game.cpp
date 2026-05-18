#include "game.h"
#include "renderer.h"
#include "logmanager/logmanager.h"
#include "../config/config.h"
#include "imgui.h"

#include <SDL_timer.h>
#include <SDL_ttf.h>

#include "imgui/imguimanager.h"
#include "input/input.h"
#include "scenemanager/scenemanager.h"
#include "assetmanager/assetmanager.h"
#include "physics/physicsmanager.h"
#include "sound/soundmanager.h"
#include "time/timer.h"

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

        //################ INIT STUFF HERE ####################

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
        if (!PhysicsManager::GetInstance().Initialise()) {
            return false;
        }
        if (!AssetManager::GetInstance().Initialise()) {
            return false;
        }
        if (!SceneManager::GetInstance().Initialise()) {
            return false;
        }

        // TODO
        // make work :(
        //SceneManager::GetInstance().onSceneSwitch.Register<Game>(&Game::ResetOrtho, *this);

        //################ INIT STUFF HERE ####################

        m_iLastTime = SDL_GetPerformanceCounter();
        m_pRenderer->SetClearColour(0, 255, 255);

        return true;
    }

    bool Game::DoGameLoop() {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {

            ImguiManager::GetInstance().ProcessEvent(event);
            InputManager::GetInstance().RegisterEvent(event);
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
        Timer::GetInstance().Tick();

        if (ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
            ToggleViewDebug();
        }

        if (!m_bIsPaused) {
            // ####### MAKE LOGIC STUFF HERE #############

            SceneManager::GetInstance().Process(deltaTime);
            PhysicsManager::GetInstance().Process(deltaTime);
            SoundManager::GetInstance().Process(deltaTime);
            InputManager::GetInstance().Process(deltaTime);

            // ###########################################
        }
        SceneManager::GetInstance().SystemProcess();
    }



    void Game::Draw(Renderer &renderer) {
        ++m_iFrameCount;
        renderer.Clear();

        // reset to default
        // allows to be overwritten by e.g camera nodes
        // ####### RENDER STUFF HERE #############

        SceneManager::GetInstance().Draw(renderer);

        // #######################################
        renderer.Draw();

#ifdef DEBUG
        if (m_bIsDebugView) {
            DrawDebug(&m_bIsDebugView);
            AssetManager::GetInstance().DrawDebug();
            PhysicsManager::GetInstance().DrawDebug();

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
            SceneManager::GetInstance().ReloadCurrentScene();
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
        if (ImGui::Begin("Stats", nullptr, window_flags))
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
            m_iFPS = static_cast<float>(m_iFrameCount);
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

    void Game::ResetOrtho() const {
        m_pRenderer->SetOrthoViewport(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
        m_pRenderer->SetOrthoOffset(0,0);
    }

    Vector2d Game::GetMouseOffset() const {
        if (m_pRenderer != nullptr) {
            return m_pRenderer->GetCurrentOffset();
        }
        return Vector2d{0,0};
    }
}
