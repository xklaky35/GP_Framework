#ifndef GP_FRAMEWORK_SCENEMANAGER_H
#define GP_FRAMEWORK_SCENEMANAGER_H

#include <map>
#include <string>

#include "../event.h"
#include "../nodes/node.h"

namespace Engine {
    class SceneManager {
    public:

        static SceneManager& GetInstance();

        void LoadAllScenes();

        bool Initialise();

        void LoadNextScene();


        void CheckForSceneSwitch();

        void Draw(Renderer & renderer);
        void Process(float delta_time);
        void SystemProcess();

        static void DestroyInstance();

        void RegisterScene(const std::string&, Node*);
        void SetSceneActive(const std::string &);
        [[nodiscard]] Node& GetCurrentVisibleNode() const;
        [[nodiscard]] const std::pmr::map<std::string, Node*>& GetScenes() const;
        Node* GetCurrentScene();

        void ReloadCurrentScene();
        void ResetWorldState();
        void DeleteScenes();

    public:
        Node* m_visibleNodeDebug;
        std::string m_currentScene;
        std::pmr::map<std::string, Node*> m_loadedScenes;

        Event<SceneManager> onSceneSwitch;

    private:
        SceneManager();
        ~SceneManager();
        static SceneManager* m_pInstance;
        bool m_bSceneSwitchOrdered;
        std::string m_nextSceneName;

    };
}



#endif //GP_FRAMEWORK_SCENEMANAGER_H