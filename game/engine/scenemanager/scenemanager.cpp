#include "scenemanager.h"
#include "imgui.h"
#include "../imgui/imguimanager.h"

namespace Engine {
    SceneManager::SceneManager() : m_visibleNodeDebug(nullptr) {}

    SceneManager::~SceneManager() {
        for (auto pair : m_loadedScenes) {
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
        assert(m_loadedScenes[m_currentScene]);
        m_loadedScenes[m_currentScene]->Init();
    }

    void SceneManager::ResetCurrentScene() {
        if (!m_currentScene.empty()) {
            m_loadedScenes[m_currentScene]->Init();
        }
    }

    Node* SceneManager::GetCurrentScene() {
        return m_loadedScenes[m_currentScene];
    }





    const std::pmr::map<std::string, Node *> &SceneManager::GetScenes() {
        return m_loadedScenes;
    }
}
