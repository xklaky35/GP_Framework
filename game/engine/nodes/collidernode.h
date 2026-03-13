#ifndef GP_FRAMEWORK_COLLIDERNODE_H
#define GP_FRAMEWORK_COLLIDERNODE_H
#include <functional>

#include "forms/form.h"
#include "node.h"
#include "spritenode.h"


enum Formtype {
    ft_CIRCLE,
    ft_RECTANGLE
};

namespace Engine {
    class ColliderNode : public Node {
    public:
        ColliderNode(Formtype, const char* = "Collider");
        ~ColliderNode() override;

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;
        void DrawDebug() override;
        void SyncFormAndSprite();
        const Form* GetForm() const;

        template<typename T>
        void RegisterOnEnter(void (T::*callback)(const Node *), T&element) {
            m_pOnEnter = std::bind(callback, &element, std::placeholders::_1);
        }

        template<typename T>
        void RegisterOnExit(void (T::*callback)(Node *), T &element) {
            m_pOnExit = std::bind(callback, element, std::placeholders::_1);
        }

    private:
        void DetectCollition() const;


    public:
        std::function<void(Node *)> m_pOnEnter;
        std::function<void(Node *)> m_pOnExit;
    private:
        Form *m_pForm;
        Formtype m_type;
        std::vector<ColliderNode *> m_collidingColliders;
        SpriteNode* m_pSpritenode;
    };
}
#endif //GP_FRAMEWORK_COLLIDERNODE_H
