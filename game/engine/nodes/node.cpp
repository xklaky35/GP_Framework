#include "node.h"

#include <random>

#include "../scenemanager/scenemanager.h"
#include "../../helper/inlinehelper.h"
#include "imgui.h"
#include "../assetmanager/assetbrowser.h"
#include "../logmanager/logmanager.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../assetmanager/assetmanager.h"

namespace  Engine {
    Node::Node() : m_Id(0),
                   m_bIsVisible(true),
                   m_parent(nullptr),
                   m_globalTransformationFlag(IF_Inherit),
                   m_iniParser(nullptr) {

        m_name = "Node";
        m_globalTransform = Transform();
        m_transform = Transform();

        m_nodeType = NT_Node;

        m_nodeInfo = {
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Node Settings");
                }
            },
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("UID");
                    ImGui::Text(n.GetUId().c_str());
                    ImGui::Separator();
                }
            },
            {
                "Name", [](Node &n) {
                    if (ImGui::InputText("##Editor", &n.m_name)) {
                        // only works by including "misc/cpp/imgui_stdlib.cpp"
                        n.SetValue("name", n.m_name);
                    }
                }
            },
            {
                "GlobalPosX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    // only make the field editable if contidion is met
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.position.x, 1, 0.5f,
                                           &v_min, &v_max)) {
                        n.SetValue("globalPosX", n.m_globalTransform.position.x);
                    }
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "GlobalPosY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.position.y, 1, 0.5f,
                                           &v_min, &v_max)) {
                        n.SetValue("globalPosY", n.m_globalTransform.position.y);
                    }
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "LocalPosX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_transform.position.x, 1, 0.5f, &v_min,
                                           &v_max)) {
                        n.SetValue("localPosX", n.m_transform.position.x);
                    };
                }
            },
            {
                "LocalPosY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_transform.position.y, 1, 0.5f, &v_min,
                                           &v_max)) {
                        n.SetValue("localPosY", n.m_transform.position.y);
                    }
                }
            },
            {
                "Base SizeX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.baseSize.x, 1, 0.5f,
                                           &v_min, &v_max)) {
                        n.SetValue("baseSizeX",n.m_globalTransform.baseSize.x);
                    }
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "Base SizeY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.baseSize.y, 1, 0.5f,
                                           &v_min, &v_max)) {
                        n.SetValue("baseSizeY",n.m_globalTransform.baseSize.y);
                    };
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "Scale", [](Node &n) {
                    int v_min = 0, v_max = 100;
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.scale, 1, 0.5f, &v_min,
                                           &v_max)) {
                        n.SetValue("scale", n.m_globalTransform.scale);
                    };
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "Rotation", [](Node &n) {
                    int v_min = 0, v_max = 100;
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.rotation, 1, 0.5f,
                                           &v_min,
                                           &v_max)) {
                        n.SetValue("rotation", n.m_globalTransform.rotation);
                    };
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "IsVisible", [](Node &n) {
                    if (ImGui::Checkbox("##Editor", &n.m_bIsVisible)) {
                        n.SetValue("isVisible", n.m_bIsVisible);
                    };
                }
            },
            {
                "InheritanceFlag", [](Node &n) {
                    if (ImGui::BeginCombo("Horizontal:", "Select container behavior")) {
                        for (int i = 0; i < 2; i++) {
                            bool is_selected =
                                    n.m_globalTransformationFlag == static_cast<InheritanceFlag>(i);
                            if (ImGui::Selectable(InheritanceFlagStrings[i], is_selected)) {
                                n.m_globalTransformationFlag = static_cast<InheritanceFlag>(i);
                                n.SetValue("inheritanceFlag", InheritanceFlagStrings[i]);
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
            }
        };
    }

    Node::~Node() {

        if (m_iniParser != nullptr) {
            m_iniParser->SaveChangesToIniFile();
            delete m_iniParser;
            m_iniParser = nullptr;
        }

        RemoveChildren();
    }

    void Node::Init() {
    }

    void Node::Process(const float deltaTime) {
        for (Node *c: m_childrenToAdd) {
            m_children.push_back(c);
        }
        m_childrenToAdd.clear();


        for (Node *c: m_childrenToDelete) {
            std::erase<Node*>(m_children, c);
            delete c;
        }
        m_childrenToDelete.clear();


        for (Node *c: m_children) {
            c->Process(deltaTime);
        }
    }

    void Node::SystemProcess() {
        // bubbles up the node tree so that the root of the element dictates the global m_position
        if (m_parent != nullptr && m_globalTransformationFlag == IF_Inherit) {
            m_globalTransform = m_parent->m_globalTransform;
            ApplyLocalTransform();
        }

        for (Node *c: m_children) {
            c->SystemProcess();
        }
    }

    void Node::Draw(Renderer &renderer) {

        for (Node *c: m_children) {
            c->Draw(renderer);
        }
    }


    void Node::DrawDebug() {

        if (ImGui::BeginTable("##bg", 1, ImGuiTableFlags_RowBg)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushID(m_Id);
            ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_None;
            tree_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            // Standard opening mode as we are likely to want to add selection afterwards
            tree_flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent; // Left arrow support
            tree_flags |= ImGuiTreeNodeFlags_SpanFullWidth; // Span full width for easier mouse reach
            tree_flags |= ImGuiTreeNodeFlags_DrawLinesToNodes; // Always draw hierarchy outlines



            bool node_open = ImGui::TreeNodeEx("", tree_flags, "%s", m_name.c_str());
            if (ImGui::BeginDragDropTarget()) {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_BROWSER_ITEMS");;

                if (payload != nullptr && payload->IsDelivery()) {

                    ImGuiID* pAssetField = (ImGuiID*)payload->Data;

                    auto items = AssetManager::GetInstance().assets;
                    for (auto& item : *items) {
                        if (item.ID == *pAssetField) {
                            LogManager::GetInstance().Log(INFO, "%s", (*items)[item.ID].AssetName.c_str());
                            Node* nodeToAdd = (*items)[item.ID].GetNode();
                            if (nodeToAdd)
                                AddChild(*nodeToAdd);
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }



            if (ImGui::IsItemFocused())
                SceneManager::GetInstance().m_visibleNodeDebug = this;
            if (node_open) {
                for (Node *c: m_children) {
                    c->DrawDebug();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
            ImGui::EndTable();
        }
    }



    void Node::AddChild(Node& node) {
        node.m_Id = ++m_Id;
        node.SetParent(this);
        m_childrenToAdd.push_back(&node);
        node.Init();
    }

    void Node::AddChildren(const std::vector<Node *>& nodes) {
        for (Node* c : nodes) {
            AddChild(*c);
        }
    }

    void Node::RemoveChild(Node* node) {
        m_childrenToDelete.push_back(node);
    }

    void Node::RemoveChildren() {
        if (m_children.empty()) return;
        for (Node* c : m_children) {
            delete c;
        }
        m_children.clear();
    }

    void Node::SetParent(Node* node) {
        m_parent = node;
    }

    const std::vector<Node*>& Node::GetChildren() const {
        return m_children;
    }

    Node * Node::GetChild(std::string name) {
        for (Node* child : m_children) {
            if (child->m_name == name)
                return child;
        }
        for (Node* child : m_childrenToAdd) {
            if (child->m_name == name)
                return child;
        }
        return nullptr;
    }


    void Node::ApplyLocalTransform() {
        // position
        m_globalTransform.position = m_parent->m_globalTransform.position + m_transform.position;

        // angle
        const Vector2d vecX(cos(m_globalTransform.GetRotationDeg()), sin(m_globalTransform.GetRotationDeg()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotationDeg()), cos(m_globalTransform.GetRotationDeg()));
        //m_globalTransform.position = (vecX * m_globalTransform.position.x) + (vecY * m_globalTransform.position.y);

        // scale
        m_globalTransform.SetScale(m_parent->m_globalTransform.GetScale() * m_transform.GetScale());

        // size
        m_globalTransform.SetHeight(m_parent->m_globalTransform.GetHeight() + m_transform.GetHeight());
        m_globalTransform.SetWidth(m_parent->m_globalTransform.GetWidth() + m_transform.GetWidth());
    }

    // only loads the configuration data from the provided file into the parser
    void Node::LoadConfigurationFile(std::string path) {
        m_UId = GenerateUID();
        m_iniParser = new IniParser();
        m_iniParser->LoadIniFile(path);
    }

    // returns all child configurations
    NodeConfiguration Node::GetChildConfiguration() {
        assert(m_iniParser);
        return m_iniParser->m_data;
    }

    // returns wether a specified child node is a custom node or not
    bool Node::IsChildCustomNodeWithId(const std::string& section) {
        assert(m_iniParser);
        return m_iniParser->GetValueAsString(section, "nodeType") == NodeTypeStrings[NT_Custom];
    }

    std::string Node::GetNameOfChildWithId(const std::string& section) {
        assert(m_iniParser);
        return m_iniParser->GetValueAsString(section, "name");
    }

    std::string Node::GetTypeOfChildWithId(std::string sectionId) {
        return m_iniParser->GetValueAsString(sectionId, "nodeType");
    }

    void Node::Setup(IniParser* parser, std::string sectionId) {
        m_UId = sectionId;
        m_globalTransformationFlag = static_cast<InheritanceFlag>(
                IniParser::GetIndexOf(
                        InheritanceFlagStrings, parser->GetValueAsString(sectionId, "inheritanceFlag").c_str(), INHERITANCE_FLAG_STRINGS_COUNT
                    )
            );
        m_globalTransform.rotation = parser->GetValueAsFloat(sectionId, "rotation");
        m_globalTransform.scale = parser->GetValueAsFloat(sectionId, "scale");
        m_globalTransform.baseSize.y = parser->GetValueAsFloat(sectionId, "baseSizeY");
        m_globalTransform.baseSize.x = parser->GetValueAsFloat(sectionId, "baseSizeX");
        m_transform.position.y = parser->GetValueAsFloat(sectionId, "localPosY");
        m_transform.position.x = parser->GetValueAsFloat(sectionId, "localPosX");
        m_globalTransform.position.y = parser->GetValueAsFloat(sectionId, "globalPosY");
        m_globalTransform.position.x = parser->GetValueAsFloat(sectionId, "globalPosX");
        m_name =  parser->GetValueAsString(sectionId, "name");
    }


    std::string Node::GetUId() {
        return m_UId;
    }

    IniParser * Node::GetIniParser() const{
        return m_iniParser;
    }

    // ################### Setter ######################

    void Node::SetValue(const std::string &key, std::string& value) {
        WriteGenericProperties();
        if (m_parent != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
    }

    void Node::SetValue(const std::string &key, const char* value) {
        WriteGenericProperties();
        if (m_parent != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
    }

    void Node::SetValue(const std::string &key, int value) {
        WriteGenericProperties();
        if (m_parent != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
    }

    void Node::SetValue(const std::string &key, float value) {
        WriteGenericProperties();
        if (m_parent != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
    }

    void Node::SetValue(const std::string &key, bool value) {
        WriteGenericProperties();
        if (m_parent != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
    }

    void Node::WriteGenericProperties() {
        if (m_parent != nullptr) {
            m_iniParser->m_data[m_UId]["nodeType"] = NodeTypeStrings[m_nodeType];
            m_iniParser->m_data[m_UId]["name"] = m_name;
        }

    }
}


