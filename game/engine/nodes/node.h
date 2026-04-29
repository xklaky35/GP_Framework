#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <functional>
#include <vector>

#include "../renderer.h"
#include "../structs/transform.h"
#include "../iniparser/iniparser.h"

namespace Engine {

    enum NodeType {
        NT_Node,
        NT_SpriteNode,
        NT_AnimatedSpriteNode,
        NT_RigidBody,
        NT_ColliderNode,
        NT_Custom
    };
    static const char* NodeTypeStrings[] = {"Node", "SpriteNode", "AnimatedSpriteNode", "RigidBody", "ColliderNode", "Custom"};



    enum InheritanceFlag {
        IF_Inherit,
        IF_Disable
    };
    static const char* InheritanceFlagStrings[] = { "Inherit", "Disable"};
    static constexpr int INHERITANCE_FLAG_STRINGS_COUNT = 2;


    struct NodeInfo;
    class Node {
    public:

        Node();

        virtual ~Node();
        virtual void Init();
        virtual void Process(float deltaTime);
        virtual void SystemProcess();
        virtual void Draw(Renderer &);
        virtual void DrawDebug();
        virtual void Setup(IniParser *parser, std::string sectionId);
        const std::vector<Node *>& GetChildren() const;
        Node* GetChild(std::string name);
        void AddChild(Node &);
        void AddChildren(const std::vector<Node *> &);
        void RemoveChild(Node *);
        void RemoveChildren();
        void SetParent(Node *);
        void ApplyLocalTransform();
        void LoadConfigurationFile(std::string path);
        NodeConfiguration GetChildConfiguration();
        bool IsChildCustomNodeWithId(const std::string& section);
        std::string GetNameOfChildWithId(const std::string& section);
        std::string GetTypeOfChildWithId(std::string sectionId);
        std::string GetUId();

        IniParser* GetIniParser() const;

        void SetValue(const std::string &key, std::string &value);
        void SetValue(const std::string &key, const char *value);
        void SetValue(const std::string &key, int value);
        void SetValue(const std::string &key, float value);
        void SetValue(const std::string &key, bool value);
        void WriteGenericProperties();

    public:
        std::string m_UId;
        std::vector<NodeInfo> m_nodeInfo;

        int m_Id; // id for ImGui::PushID(m_Id);
        std::string m_name;
        bool m_bIsVisible;;
        Node* m_parent;

        // Flag indicating if this node inherits transformation information from the parent node
        InheritanceFlag m_globalTransformationFlag;

        NodeType m_nodeType;
        //Local transformation in the scope of the parent node
        Transform m_transform;
        //Global transformation using world coordinates
        Transform m_globalTransform;


    protected:
        std::vector<Node*> m_children;
        std::vector<Node*> m_childrenToAdd;
        std::vector<Node*> m_childrenToDelete;
        IniParser* m_iniParser;
    };

    struct NodeInfo
    {
        const char*     Name;       // Member name
        std::function<void(Node&)> Draw;
    };


}
#endif
