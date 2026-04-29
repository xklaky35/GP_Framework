#ifndef GP_FRAMEWORK_SPLASHSCREEN_H
#define GP_FRAMEWORK_SPLASHSCREEN_H

#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"
#include "../../engine/nodes/ui/control.h"

using namespace Engine;

class Splashscreen : public Control {
public:
    Splashscreen();
    ~Splashscreen() override;
    void Init() override;
    void Process(float deltaTime) override;


private:
    float m_fFadeSpeed;
    float m_fCurrentAlphaLevel;
    float m_windowWidth;
    float m_windowHeight;
    int m_iCurrentScreen;
    std::vector<SpriteNode*> m_screens;

    SpriteNode* m_background;
    SpriteNode* m_autLogo;
    SpriteNode* m_fmodLogo;
};


#endif