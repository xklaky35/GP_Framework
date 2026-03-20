#ifndef GP_FRAMEWORK_COLLIDERNODE_H
#define GP_FRAMEWORK_COLLIDERNODE_H
#include <functional>

#include "forms/form.h"
#include "node.h"
#include "spritenode.h"
#include "../event.h"


enum Formtype {
    ft_CIRCLE,
    ft_RECTANGLE
};

namespace Engine {
    class ColliderNode : public Node {
    public:
        ColliderNode(Formtype);

        void Init() override;
        void Process(float deltaTime) override;
        void SystemProcess() override;
        void Draw(Renderer &) override;
        void DrawDebug() override;
        void UpdateForm();
        const Form* GetForm() const;

    private:
        void DetectCollition() const;


    public:
        Event<Node> OnCollision;

    private:
        Form *m_pForm;
        Formtype m_type;
        std::vector<ColliderNode *> m_collidingColliders;
        SpriteNode* m_pSpritenode;
    };
}
#endif //GP_FRAMEWORK_COLLIDERNODE_H
