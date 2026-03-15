#include "scenemanager.h"
#include "imgui.h"
#include "../imgui/imguimanager.h"

namespace Engine {
    SceneManager::SceneManager() = default;

    SceneManager::~SceneManager() {
        for (auto& pair : m_loadedScenes) {
            delete pair.second;
            pair.second = nullptr;
        }
        m_loadedScenes.clear();
    }

    SceneManager* SceneManager::m_pInstance = nullptr;

    SceneManager & SceneManager::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new SceneManager();
        }
        return *m_pInstance;
    }

    void SceneManager::DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void SceneManager::RegisterScene(const std::string& sceneName, Node * scene) {
        m_loadedScenes[sceneName] = scene;
    }

    void SceneManager::LoadScene(const std::string& sceneName) {
        m_currentScene = sceneName;
        m_loadedScenes[m_currentScene]->Init();
    }

    Node* SceneManager::GetCurrentScene() {
        return m_loadedScenes[m_currentScene];
    }



    void SceneManager::DrawDebug(bool* p_open) {
        ImGui::Begin("Scene Manager", p_open, ImGuiWindowFlags_MenuBar);
        ImGui::BeginMultiSelect(ImGuiMultiSelectFlags_NoSelectAll, m_loadedScenes.size()-1, m_loadedScenes.size()-1);

        for (const auto&[sceneName, scene] : m_loadedScenes) {
            if (ImGui::Selectable(sceneName.c_str())) {
                LoadScene(sceneName);
            }
        }

        ImGui::EndMultiSelect();
        ImGui::End();
    }

    const std::pmr::map<std::string, Node *> & SceneManager::GetScenes() {
        return m_loadedScenes;
    }
}
