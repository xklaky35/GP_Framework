#include "scenemanager.h"
#include "imgui.h"
#include "../event.h"
#include "../../scenes/mainmenu/mainmenu.h"
#include "../../scenes/slashscreen/splashscreen.h"
#include "../../scenes/whoosh/scenewhoosh.h"
#include "../physics/physicsmanager.h"

namespace Engine {


    SceneManager* SceneManager::m_pInstance = nullptr;
    SceneManager::SceneManager() : m_visibleNodeDebug(nullptr) {
        onSceneSwitch = Event<SceneManager>();
        m_bSceneSwitchOrdered = false;
    }
    SceneManager::~SceneManager() {
        for (auto pair : m_loadedScenes) {
            delete pair.second;
            pair.second = nullptr;
        }
        m_loadedScenes.clear();
    }

    void SceneManager::LoadAllScenes() {
        RegisterScene("Splash", new Splashscreen());
        RegisterScene("MainMenu", new MainMenu());
        RegisterScene("Whoosh", new SceneWhoosh());
    }



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



    bool SceneManager::Initialise() {
        LoadAllScenes();
        SetSceneActive("Splash");
        LoadNextScene();
        return true;
    }

    void SceneManager::CheckForSceneSwitch() {
        if (m_bSceneSwitchOrdered == false) return;
        LoadNextScene();
    }

    void SceneManager::Draw(Renderer &renderer) {
        m_loadedScenes[m_currentScene]->Draw(renderer);
        CheckForSceneSwitch();
    }
    void SceneManager::Process(float deltaTime) {
        m_loadedScenes[m_currentScene]->Process(deltaTime);
        CheckForSceneSwitch();

    }
    void SceneManager::SystemProcess() {
        m_loadedScenes[m_currentScene]->SystemProcess();
        CheckForSceneSwitch();
    }


    void SceneManager::SetSceneActive(std::string sceneName) {
        m_nextSceneName = sceneName;
        m_bSceneSwitchOrdered = true;
    }

    void SceneManager::LoadNextScene() {
        ResetWorldState();
        m_currentScene = m_nextSceneName;
        assert(m_loadedScenes[m_currentScene]);
        m_visibleNodeDebug = GetCurrentScene();
        m_loadedScenes[m_currentScene]->Init();
        m_bSceneSwitchOrdered = false;
    }

    void SceneManager::ReloadCurrentScene() {
        SetSceneActive(m_currentScene);
    }

    Node& SceneManager::GetCurrentVisibleNode() {
        return *m_visibleNodeDebug;
    }



    void SceneManager::RegisterScene(const std::string& sceneName, Node * scene) {
        m_loadedScenes[sceneName] = scene;
    }

    void SceneManager::ResetWorldState() {
        DeleteScenes();
        PhysicsManager::GetInstance().ResetGameWorld();
        LoadAllScenes();
    }

    void SceneManager::DeleteScenes() {
        for (auto [sceneName, scene]: m_loadedScenes) {
            delete m_loadedScenes[sceneName];
            m_loadedScenes[sceneName] = nullptr;
        }
        m_loadedScenes.clear();
    }

    Node* SceneManager::GetCurrentScene() {
        return m_loadedScenes[m_currentScene];
    }



    const std::pmr::map<std::string, Node *> &SceneManager::GetScenes() {
        return m_loadedScenes;
    }
}
