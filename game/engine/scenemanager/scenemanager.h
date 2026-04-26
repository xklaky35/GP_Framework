#ifndef GP_FRAMEWORK_SCENEMANAGER_H
#define GP_FRAMEWORK_SCENEMANAGER_H

#include <map>
#include <string>
#include "../imgui/imguiwindowbaseclass.h"

#include "../nodes/node.h"

namespace Engine {
    class SceneManager {
    public:

        static SceneManager& GetInstance();
        static void DestroyInstance();
        void RegisterScene(const std::string&, Node*);
        void LoadScene(const std::string&);
        Node* GetCurrentScene();
        const std::pmr::map<std::string, Node*>& GetScenes();
        void ResetCurrentScene();

    public:
        Node* m_visibleNodeDebug;
        std::string m_currentScene;
        std::pmr::map<std::string, Node*> m_loadedScenes;

    private:
        SceneManager();
        ~SceneManager();
        static SceneManager* m_pInstance;

    };
}



#endif //GP_FRAMEWORK_SCENEMANAGER_H