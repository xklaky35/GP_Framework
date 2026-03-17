#include "splashscreen.h"

#include "../../config/config.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0), m_iCurrentScreen(0), m_background(nullptr) {}
Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Node::Init();

    int dimY = Config::GetInstance().windowsHeight;
    int dimX = Config::GetInstance().windowsWidth;

    m_globalTransform.position.x = dimX/2;
    m_globalTransform.position.y = dimY/2;

    m_background = new SpriteNode("../assets/Sprites/rect.png");
    m_background->m_spriteDisplayMode = Fit;
    m_background->m_transform.SetSize(dimX, dimY);
    m_background->SetRGBA(0,0,0,1);
    AddChild(*m_background);

    auto* aut = new SpriteNode("../assets/Splash/aut.png");
    aut->SetRGBA(1,1,1,0.01);
    m_screens.push_back(aut);
    aut->m_transform.SetScale(1.8);
    AddChild(*aut);

    auto* kiwi = new SpriteNode("../assets/Splash/kiwi.png");
    kiwi->SetRGBA(1,1,1,0.01);
    m_screens.push_back(kiwi);
    kiwi->m_transform.SetScale(0.7);
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
