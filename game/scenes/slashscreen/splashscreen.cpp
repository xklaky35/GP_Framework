#include "splashscreen.h"

#include "../../config/config.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_background(nullptr), m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0), m_iCurrentScreen(0) {
}

Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Node::Init();

    int dimY = Config::GetInstance().windowsHeight;
    int dimX = Config::GetInstance().windowsWidth;

    m_background = new SpriteNode("../assets/Sprites/rect.png", "Background");
    m_background->m_bUseSpriteSize = false;
    m_background->SetRGBA(0,0,0,1);
    m_background->m_transform->height = dimY;
    m_background->m_transform->width = dimX;
    m_background->m_transform->position->x = dimX/2;
    m_background->m_transform->position->y = dimY/2;
    AddChild(*m_background);

    auto* aut = new SpriteNode("../assets/Splash/aut.png", "AUT Splash");
    aut->SetRGBA(1,1,1,0.01);
    aut->m_transform->scale = 1.8;
    aut->m_transform->position->x = dimX/2;
    aut->m_transform->position->y = dimY/2;
    m_screens.push_back(aut);
    AddChild(*aut);

    auto* kiwi = new SpriteNode("../assets/Splash/kiwi.png", "Kiwi Splash");
    kiwi->SetRGBA(1,1,1,0.01);
    kiwi->m_transform->scale = 0.8;
    kiwi->m_transform->position->x = dimX/2;
    kiwi->m_transform->position->y = dimY/2;
    m_screens.push_back(kiwi);
    AddChild(*kiwi);
}

void Splashscreen::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_iCurrentScreen >= m_screens.size()) {
        return;
    }

    m_screens[m_iCurrentScreen]->SetRGBA(1,1,1, m_fCurrentAlphaLevel +=  m_fFadeSpeed * deltaTime);

    if (m_fCurrentAlphaLevel >= 1) {
        m_fFadeSpeed *= -1;
    }
    if (m_fCurrentAlphaLevel <= 0) {
        m_fCurrentAlphaLevel = 0;
        m_fFadeSpeed *= -1;
        m_screens[m_iCurrentScreen++]->SetRGBA(1,1,1,0);
    }
}
