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




        ImGui::Begin("Nodes");
        if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F, ImGuiInputFlags_Tooltip);
            m_loadedScenes[m_currentScene]->DrawDebug();
        }
        ImGui::EndChild();
        ImGui::End();


        ImGui::Begin("NodesProperties");
        //ImGui::SameLine();
        ImGui::BeginGroup(); // Lock X position
        if (m_visibleNodeDebug) {
            ImGui::Text("%s", m_visibleNodeDebug->m_name);
            ImGui::TextDisabled("UID: 0x%08X", m_visibleNodeDebug->m_Id);
            ImGui::Separator();
            if (ImGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
                // Push object ID after we entered the table, so table is shared for all objects
                ImGui::PushID((int) m_visibleNodeDebug->m_Id);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
                for (const NodeInfo &field_desc: m_visibleNodeDebug->m_nodeInfo) {
                    ImGui::TableNextRow();
                    ImGui::PushID(field_desc.Name);
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    ImGui::TextUnformatted(field_desc.Name);
                    ImGui::TableNextColumn();

                    field_desc.Draw(*m_visibleNodeDebug);
                    ImGui::PopID();
                }
                ImGui::PopID();
                ImGui::EndTable();
            }
        }
        ImGui::EndGroup(); // Lock X position
        ImGui::End();
    }

    const std::pmr::map<std::string, Node *> &SceneManager::GetScenes() {
        return m_loadedScenes;
    }
}
