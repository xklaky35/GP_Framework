#include "splashscreen.h"

#include "../../config/config.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0), m_iCurrentScreen(0), m_background(nullptr) {
}
Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Node::Init();

    m_bIsRoot = true;

    int dimY = Config::GetInstance().windowsHeight;
    int dimX = Config::GetInstance().windowsWidth;

    m_background = new SpriteNode("../assets/Sprites/rect.png", "Background");
    m_background->m_bUseSpriteSize = false;
    m_background->SetRGBA(0,0,0,1);
    m_background->m_transform->height = dimY;
    m_background->m_transform->width = dimX;
    m_background->m_position->x = dimX/2;
    m_background->m_position->y = dimY/2;
    AddChild(*m_background);

    auto* aut = new SpriteNode("../assets/Splash/aut.png", "AUT Splash");
    aut->SetRGBA(1,1,1,0.01);
    aut->m_transform->scale = 1.8;
    aut->m_position->x = dimX/2;
    aut->m_position->y = dimY/2;
    m_screens.push_back(aut);
    AddChild(*aut);

    auto* kiwi = new SpriteNode("../assets/Splash/kiwi.png", "Kiwi Splash");
    kiwi->SetRGBA(1,1,1,0.01);
    kiwi->m_transform->scale = 0.8;
    kiwi->m_position->x = dimX/2;
    kiwi->m_position->y = dimY/2;
    m_screens.push_back(kiwi);
    AddChild(*kiwi);
}

void Splashscreen::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_iCurrentScreen >= m_screens.size()) {
        SceneManager::GetInstance().LoadScene("Game");
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
