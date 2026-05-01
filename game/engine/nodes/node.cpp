#include "node.h"

#include <random>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "rigidbodynode.h"
#include "../logmanager/logmanager.h"
#include "../../helper/inlinehelper.h"
#include "../assetmanager/assetbrowser.h"
#include "../assetmanager/assetmanager.h"
#include "../physics/physicsmanager.h"
#include "../scenemanager/scenemanager.h"

namespace  Engine {
    Node::Node() : m_Id(0),
                   m_bIsVisible(true),
                   m_parent(nullptr),
                   m_globalTransformationFlag(IF_Inherit),
                   m_iniParser(nullptr) {

        m_UId = GenerateUID();
        m_globalTransform = Transform();
        m_transform = Transform();

        SetupNode("Node", NT_Node);

        m_nodeInfo = {
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Node Settings");
                }
            },
            {
                "", [](const Node &n) {
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
                "GlobalPos X - Y", [](Node &n) {

                    // only make the field editable if condition is met
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    Vector2d tmpGlobalPos = n.GetGlobalPosition();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpGlobalPos.x, 2, 0.5f, nullptr, nullptr)) {

                        n.SetValue("globalPosX", tmpGlobalPos.x);
                        n.SetValue("globalPosY", tmpGlobalPos.y);
                        n.SetGlobalPosition(tmpGlobalPos);
                    }
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "LocalPos X - Y", [](Node &n) {
                    Vector2d tmpLocalPos = n.GetLocalPos();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpLocalPos, 2, 0.5f, nullptr,
                                           nullptr)) {
                        n.SetValue("localPosX", tmpLocalPos.x);
                        n.SetValue("localPosY", tmpLocalPos.y);
                        n.SetLocalPosition(tmpLocalPos);
                    };
                }
            },
            {
                "Base Size", [](Node &n) {
                    Vector2d tmpBaseSize = n.GetBaseSize();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpBaseSize, 2, 0.5f,
                                           nullptr, nullptr)) {
                        n.SetValue("baseSizeX",tmpBaseSize.x);
                        n.SetValue("baseSizeY",tmpBaseSize.y);
                        n.SetBaseSize(tmpBaseSize);
                    }
                }
            },
            {
                "Scale", [](Node &n) {
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.scale, 1, 0.5f, nullptr,
                                           nullptr)) {
                        n.SetValue("scale", n.m_globalTransform.scale);
                    };
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "Rotation", [](Node &n) {
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::BeginDisabled();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.rotation, 1, 0.5f,
                                           nullptr,
                                           nullptr)) {
                        n.SetValue("rotation", n.m_globalTransform.rotation);
                    };
                    if (n.m_globalTransformationFlag == IF_Inherit) ImGui::EndDisabled();
                }
            },
            {
                "IsVisible", [](Node &n) {
                    bool tmpIsVisible = n.IsVisible();
                    if (ImGui::Checkbox("##Editor", &tmpIsVisible)) {
                        n.SetValue("isVisible", tmpIsVisible);
                        n.SetVisibility(tmpIsVisible);
                    };
                }
            },
            {
                "InheritanceFlag", [](Node &n) {
                    if (ImGui::BeginCombo("", "Select inheritance flag")) {
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
            m_globalTransform.position = m_parent->m_globalTransform.position;
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

                    const auto* pAssetField = static_cast<ImGuiID *>(payload->Data);
                    for (auto items = AssetManager::GetInstance().assets; auto& item : *items) {
                        if (item.ID == *pAssetField) {
                            LogManager::GetInstance().Log(INFO, "%s", (*items)[item.ID].AssetName.c_str());
                            if (Node* nodeToAdd = (*items)[item.ID].GetNode())
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



    void Node::SetupParameter(IniParser* parser, const std::string& sectionId) {
        m_UId = sectionId;
        m_globalTransformationFlag = static_cast<InheritanceFlag>(
            GetIndexOf(
                InheritanceFlagStrings, parser->GetValueAsString(sectionId, "inheritanceFlag").c_str(), INHERITANCE_FLAG_STRINGS_COUNT
            )
        );
        m_globalTransform.rotation = parser->GetValueAsFloat(sectionId, "rotation");
        m_globalTransform.scale = parser->GetValueAsFloat(sectionId, "scale");
        m_globalTransform.baseSize.y = parser->GetValueAsFloat(sectionId, "baseSizeY");
        m_globalTransform.baseSize.x = parser->GetValueAsFloat(sectionId, "baseSizeX");
        m_transform.position.y = parser->GetValueAsFloat(sectionId, "localPosY");
        m_transform.position.x = parser->GetValueAsFloat(sectionId, "localPosX");
        m_name =  parser->GetValueAsString(sectionId, "name");

        Vector2d pos;
        pos.x = parser->GetValueAsFloat(sectionId, "globalPosX");
        pos.y = parser->GetValueAsFloat(sectionId, "globalPosY");
        SetGlobalPosition(pos);
    }


    Vector2d Node::GetGlobalPosition() const {
        auto* r = dynamic_cast<RigidbodyNode*>(CheckForRigidbodyNode());
        if (r != nullptr) {
            // read meters back from Box2D, convert to pixels for the sprite

            return r->GetBodyPosition();
        }
        return m_globalTransform.position;  // plain nodes just return pixels
    }

    void Node::SetGlobalPosition(const Vector2d pos) {
        auto* r = dynamic_cast<RigidbodyNode*>(CheckForRigidbodyNode());
        if (r != nullptr) {
            r->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
        }
        else {
            m_globalTransform.position = pos;
        }
    }



    void Node::SetLocalPosition(const Vector2d pos) {
        m_transform.position = pos;
    }

    const std::vector<Node*>& Node::GetChildren() const {
        return m_children;
    }

    Node * Node::GetChild(const std::string &name) const {
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

    // if a rigidbody node is present we need to use its physics api insted to set the position
    Node* Node::CheckForRigidbodyNode() const {
        for (Node* child : m_children) {
            if (child->m_nodeType == NT_RigidBodyNode)
                return child;
        }
        for (Node* child : m_childrenToAdd) {
            if (child->m_nodeType == NT_RigidBodyNode)
                return child;
        }
        return nullptr;
    }

    void Node::AddChild(Node& node) {
        node.m_Id = ++m_Id;
        node.SetParent(this);
        m_childrenToAdd.push_back(&node);
        WriteGenericProperties();
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

    void Node::SetVisibility(bool isVisible) {
        m_bIsVisible = isVisible;
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

    }

    // only loads the configuration data from the provided file into the parser
    void Node::LoadConfigurationFile(const std::string &path) {
        m_iniParser = new IniParser();
        m_iniParser->LoadIniFile(path);
    }

    void Node::SetupNode(const std::string& typeName, const NodeType nodeType) {
        m_typeName = typeName;
        m_nodeType = nodeType;
    }

    // returns all child configurations
    NodeConfiguration Node::GetChildConfiguration() const {
        assert(m_iniParser);
        return m_iniParser->m_data;
    }

    // returns wether a specified child node is a custom node or not
    bool Node::IsChildCustomNodeWithId(const std::string& section) const {
        assert(m_iniParser);
        return m_iniParser->GetValueAsString(section, "nodeType") == NodeTypeStrings[NT_Custom];
    }

    std::string Node::GetNameOfChildWithId(const std::string& section) const {
        assert(m_iniParser);
        return m_iniParser->GetValueAsString(section, "typeName");
    }

    std::string Node::GetTypeOfChildWithId(const std::string& sectionId) const {
        return m_iniParser->GetValueAsString(sectionId, "nodeType");
    }


    std::string Node::GetUId() const {
        return m_UId;
    }

    IniParser * Node::GetIniParser() const{
        return m_iniParser;
    }

    // ################### Setter ######################

    void Node::SetValue(const std::string &key, std::string& value) {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
        WriteGenericProperties();
    }

    void Node::SetValue(const std::string &key, const char* value) {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
        WriteGenericProperties();
    }

    void Node::SetValue(const std::string &key, const int value) {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
        WriteGenericProperties();
    }

    void Node::SetValue(const std::string &key, const float value) {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
        WriteGenericProperties();
    }

    void Node::SetValue(const std::string &key, const bool value) {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, key, value);
        }
        WriteGenericProperties();
    }

    void Node::WriteGenericProperties() {
        if (m_parent != nullptr && m_parent->m_iniParser != nullptr) {
            m_parent->m_iniParser->SetValue(m_UId, "nodeType", NodeTypeStrings[m_nodeType]);
            m_parent->m_iniParser->SetValue(m_UId, "typeName", m_typeName);
            m_parent->m_iniParser->SetValue(m_UId, "name", m_name);
        }

    }

    Vector2d Node::GetLocalPos() const {
        return m_transform.position;
    }

    bool Node::IsVisible() const {
        return m_bIsVisible;
    }

    void Node::SetBaseSize(const Vector2d baseSize) {
        m_globalTransform.baseSize = baseSize;
    }

    Vector2d Node::GetBaseSize() const {
        return m_globalTransform.baseSize;
    }
}


