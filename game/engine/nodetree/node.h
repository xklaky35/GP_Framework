#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <string>
#include <vector>

#include "../renderer.h"
#include "../structs/transform.h"
#include "../structs/vector2d.h"

namespace Engine {

    class Node {
    public:
        Node(const char * = "Node");
        virtual ~Node();
        virtual void Init();
        virtual void Process(float deltaTime);
        virtual void Draw(Renderer &);
        Node *GetChild(const char *) const;
        std::vector<Node *> GetChildren() const;
        void AddChild(Node &);
        void AddChildren(const std::vector<Node *> &);
        void RemoveChild(Node *);
        void RemoveChildren();
        void SetParent(Node *);


    public:
        std::string name;
        bool m_bIsRoot;

        // local node transform
        Transform *m_transform;
        // global element position
        Vector2d *m_position;

    private:
        Node *parent;
        std::vector<Node *> children;
    };
}
#endif
