#ifndef GP_FRAMEWORK_SCENEMANAGER_H
#define GP_FRAMEWORK_SCENEMANAGER_H

#include <map>
#include <string>

#include "../event.h"
#include "../imgui/imguiwindowbaseclass.h"

#include "../nodes/node.h"

namespace Engine {
    class SceneManager {
    public:

        static SceneManager& GetInstance();

        void LoadAllScenes();

        bool Initialise();

        static void DestroyInstance();
        void RegisterScene(const std::string&, Node*);
        void SetSceneActive(std::string);
        Node& GetCurrentVisibleNode();
        Node* GetCurrentScene();
        const std::pmr::map<std::string, Node*>& GetScenes();
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

    };
}



#endif //GP_FRAMEWORK_SCENEMANAGER_H