#include "node.h"

#include "imgui.h"
#include "../scenemanager/scenemanager.h"
#include "../logmanager/logmanager.h"

namespace  Engine {


    Node::Node(const char *nodeName) : m_name(nodeName),
                                       m_globalTransformationFlag(Inherit),
                                       m_parent(nullptr),
                                       m_Id(0),
                                       m_bIsVisible(true){





        m_nodeInfo = {
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Node Settings");
                }
            },
            {
                "Name", [](Node &n) {
                    ImGui::InputText("##Editor", const_cast<char *>(n.m_name), 28);
                }
            },
            {
                "GlobalPosX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.position.x, 1, 0.5f, &v_min, &v_max);
                }
            },
            {
                "GlobalPosY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.position.y, 1, 0.5f, &v_min, &v_max);
                }
            },
            {
                "LocalPosX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_transform.position.x, 1, 0.5f,
                                       &v_min, &v_max);
                }
            },
            {
                "LocalPosY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_transform.position.y, 1, 0.5f,
                                       &v_min, &v_max);
                }
            },
            {
                "SizeX", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.scaledSize.x, 1, 0.5f,
                                       &v_min, &v_max);
                }
            },
            {
                "SizeY", [](Node &n) {
                    int v_min = -10000, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &n.m_globalTransform.scaledSize.y, 1, 0.5f,
                                       &v_min, &v_max);
                }
            },
            {
                "IsVisible", [](Node &n) {
                    ImGui::Checkbox("##Editor", &n.m_bIsVisible);
                }
            },
        };

        m_globalTransform = Transform();
        m_transform = Transform();
    }

    Node::~Node() {
        RemoveChildren();
    }

    void Node::Init() {
    }

    void Node::Process(const float deltaTime) {
        for (Node *c: m_children) {
            c->Process(deltaTime);
        }
    }

    void Node::SystemProcess() {
        // bubbles up the node tree so that the root of the element dictates the global m_position
        if (m_parent != nullptr && m_globalTransformationFlag == Inherit) {
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
            bool node_open = ImGui::TreeNodeEx("", tree_flags, "%s", m_name);
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
        m_children.push_back(&node);
        node.Init();
    }

    void Node::AddChildren(const std::vector<Node *>& nodes) {
        for (Node* c : nodes) {
            AddChild(*c);
        }
    }

    void Node::RemoveChild(Node* node) {
        std::erase<Node*>(m_children, node);
        delete node;
    }

    void Node::RemoveChildren() {
        if (m_children.empty()) return;
        for (const Node* c : m_children) {
            delete c;
            c = nullptr;
        }
        m_children.clear();
    }

    void Node::SetParent(Node* node) {
        m_parent = node;
    }

    const std::vector<Node*>& Node::GetChildren() const {
        return m_children;
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
}


