#ifndef GP_FRAMEWORK_SPLASHSCREEN_H
#define GP_FRAMEWORK_SPLASHSCREEN_H

#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"

using namespace Engine;

class Splashscreen : public Node {
public:
    Splashscreen();
    ~Splashscreen();
    void Init() override;
    void Process(float deltaTime) override;


private:
    float m_fFadeSpeed;
    float m_fCurrentAlphaLevel;
    int m_iCurrentScreen;
    std::vector<SpriteNode*> m_screens;
    SpriteNode* m_background;
};


#endif