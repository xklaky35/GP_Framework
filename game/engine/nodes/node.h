#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <functional>
#include <vector>

#include "../renderer.h"
#include "../iniparser/iniparser.h"
#include "../structs/transform.h"

namespace Engine {
    enum NodeType : int;

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
        virtual void SetupParameter(IniParser *parser, const std::string &sectionId);


        void AddChild(Node &);
        void AddChildren(const std::vector<Node *> &);
        void RemoveChild(Node *);
        void RemoveChildren();

        // getter
        std::string GetUId() const;
        bool IsVisible() const;
        Vector2d GetLocalPos() const;
        Vector2d GetBaseSize() const;
        IniParser* GetIniParser() const;
        Vector2d GetGlobalPosition() const;
        Node* GetChild(const std::string &name) const;
        const std::vector<Node *>& GetChildren() const;
        NodeConfiguration GetChildConfiguration() const;
        bool IsChildCustomNodeWithId(const std::string& section) const;
        std::string GetNameOfChildWithId(const std::string& section) const;
        std::string GetTypeOfChildWithId(const std::string &sectionId) const;

        // setter
        void SetParent(Node *);
        void SetVisibility(bool isVisible);
        void SetBaseSize(Vector2d baseSize);
        void SetLocalPosition(Vector2d pos);
        void SetGlobalPosition(Vector2d pos);
        void SetValue(const std::string &key, int value);
        void SetValue(const std::string &key, bool value);
        void SetValue(const std::string &key, float value);
        void SetValue(const std::string &key, const char *value);
        void SetValue(const std::string &key, std::string &value);

        // misc
        void ApplyLocalTransform();
        void LoadConfigurationFile(const std::string &path);
        void SetupNode(const std::string &typeName, NodeType nodeType);
        Node *CheckForRigidbodyNode() const;
        void WriteGenericProperties();


    public:
        std::string m_UId;
        std::vector<NodeInfo> m_nodeInfo;

        int m_Id; // id for ImGui::PushID(m_Id);

        std::string m_name;
        std::string m_typeName;
        NodeType m_nodeType;

        //Local transformation in the scope of the parent node
        Transform m_transform;
        //Global transformation using world coordinates
        Transform m_globalTransform;

        bool m_bIsVisible;;
        Node* m_parent;

        // Flag indicating if this node inherits transformation information from the parent node
        InheritanceFlag m_globalTransformationFlag;


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
