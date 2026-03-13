#include "imguimanager.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace Engine {

    ImguiManager *ImguiManager::m_pInstance = nullptr;

    ImguiManager::ImguiManager(): m_bShowDemoWindow(false) {}
    ImguiManager::~ImguiManager() = default;

    ImguiManager&  ImguiManager::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new ImguiManager();
        }
        return *m_pInstance;
    }

    void  ImguiManager::DestroyInstance() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void ImguiManager::Initialize(SDL_Window *window, SDL_GLContext context) {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init();
    }

    void ImguiManager::ProcessEvent(SDL_Event event) {
        ImGui_ImplSDL2_ProcessEvent(&event); // Forward your event to backend
    }

    // Start New Frame
    void ImguiManager::Process() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        if (m_bShowDemoWindow) {
            ImGui::ShowDemoWindow();
        }
    }

    // renders frame
    void ImguiManager::Draw() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
