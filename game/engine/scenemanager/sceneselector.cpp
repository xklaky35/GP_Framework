#include "sceneselector.h"

#include "imgui.h"
#include "scenemanager.h"
#include "../assetmanager/assetbrowser.h"
#include "../logmanager/logmanager.h"


namespace Engine {
    SceneSelector::SceneSelector(bool visible) {
        name = "SceneSelector";
        this->visible = visible;
    }

    void SceneSelector::DrawDebug() {

        SceneManager& sceneManager = SceneManager::GetInstance();

        bool p_open;
        // ########## Scene selector ##############
        ImGui::Begin(name.c_str(), &p_open, ImGuiWindowFlags_MenuBar);
        ImGui::BeginMultiSelect(ImGuiMultiSelectFlags_NoSelectAll, sceneManager.m_loadedScenes.size()-1, sceneManager.m_loadedScenes.size()-1);

        for (const auto&[sceneName, scene] : sceneManager.m_loadedScenes) {
            if (ImGui::Selectable(sceneName.c_str())) {
                sceneManager.LoadScene(sceneName);
            }
        }

        ImGui::EndMultiSelect();
        ImGui::End();



        // ########### Node Tree ###################
        ImGui::Begin("Nodes");

        if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F, ImGuiInputFlags_Tooltip);
            sceneManager.m_loadedScenes[sceneManager.m_currentScene]->DrawDebug();
        }
        ImGui::EndChild();
        ImGui::End();


        // ########## Selected node properties
        ImGui::Begin("NodesProperties");
        //ImGui::SameLine();
        ImGui::BeginGroup(); // Lock X position
        if (sceneManager.m_visibleNodeDebug) {
            ImGui::Text("%s", sceneManager.m_visibleNodeDebug->m_name.c_str());
            ImGui::TextDisabled("UID: 0x%08X", sceneManager.m_visibleNodeDebug->m_Id);
            ImGui::Separator();
            if (ImGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
                // Push object ID after we entered the table, so table is shared for all objects
                ImGui::PushID((int) sceneManager.m_visibleNodeDebug->m_Id);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
                for (const NodeInfo &field_desc: sceneManager.m_visibleNodeDebug->m_nodeInfo) {
                    /*
                    if (sceneManager.m_visibleNodeDebug->m_iniParser == nullptr) {
                        continue;
                    }
                    */
                    ImGui::TableNextRow();
                    ImGui::PushID(field_desc.Name);
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    ImGui::TextUnformatted(field_desc.Name);
                    ImGui::TableNextColumn();

                    field_desc.Draw(*sceneManager.m_visibleNodeDebug);
                    ImGui::PopID();
                }
                ImGui::PopID();
                ImGui::EndTable();
            }
        }
        ImGui::EndGroup(); // Lock X position
        ImGui::End();

    }
}
