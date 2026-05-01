#include "assetmanager.h"

#include <filesystem>

#include "../scenemanager/sceneselector.h"

#include "../logmanager/logmanager.h"
#include "../nodes/animatedspritenode.h"
#include "../nodes/rigidbodynode.h"

namespace Engine {

    AssetManager::AssetManager() : assets(nullptr) {}

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


    bool AssetManager::Initialise() {

        auto* assetBrowser = new AssetBrowser(false);
        auto* sceneSelector = new SceneSelector(false);

        if (!LoadAssets((assetBrowser))) {
            return false;
        }

        windows.push_back(assetBrowser);
        windows.push_back(sceneSelector);

        return true;
    }

    bool AssetManager::LoadAssets(AssetBrowser* assetBrowser) {
        assetBrowser->ClearItems();

        // looks for .ini files in the projects directory
        // this loads all assets of type "Component"
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(PROJECT_ROOT)) {
                // Check if the current entry is a regular file
                if (std::filesystem::is_regular_file(entry.path())) {
                    if (entry.path().extension().compare(".ini") == 0)
                        assetBrowser->AddItem(AssetField(AT_Component, entry.path()));
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            LogManager::GetInstance().Log(ERROR, "Error: %s", e.what());
            return false;
        }


        // load all assets of type "Node"
        assetBrowser->AddItem(AssetField(AT_Node, NT_Node));
        assetBrowser->AddItem(AssetField(AT_Node, NT_RigidBodyNode));
        assetBrowser->AddItem(AssetField(AT_Node, NT_SpriteNode));
        assetBrowser->AddItem(AssetField(AT_Node, NT_AnimatedSpriteNode));
        assetBrowser->AddItem(AssetField(AT_Node, NT_CameraNode));

        assets = &assetBrowser->Items;
        return true;
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
