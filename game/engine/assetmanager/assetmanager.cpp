#include "assetmanager.h"
#include "assetbrowser.h"
#include "sceneeditor.h"
#include "../scenemanager/sceneselector.h"

#include "imgui.h"

namespace Engine {

    AssetManager::AssetManager() {

        windows.push_back(new AssetBrowser(false));
        windows.push_back(new SceneEditor(false));
        windows.push_back(new SceneSelector(true));

    }

    AssetManager::~AssetManager() = default;

    AssetManager* AssetManager::m_pInstance = nullptr;
    AssetManager& AssetManager::GetInstance() {
         if (m_pInstance == nullptr) {
             m_pInstance = new AssetManager();
         }
        return *m_pInstance;
    }

    void AssetManager::DeleteInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void AssetManager::DrawDebug() const {

        ImGui::Begin("Windows");
        ImGui::Separator();

        for (auto w : windows)
        {
            ImGui::Checkbox(w->name.c_str(), &w->visible);
        }

        ImGui::End();

        // --- Draw visible windows ---
        for (auto w : windows)
        {
            if (!w->visible) continue;
            w->DrawDebug();
        }
    }


}
