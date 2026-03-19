#include "splashscreen.h"
#include "../../config/config.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0),
                               m_iCurrentScreen(0),
                               m_background(nullptr),
                               m_aut(nullptr),
                               m_kiwi(nullptr) {
    m_windowHeight = static_cast<float>(Config::GetInstance().windowsHeight);
    m_windowWidth = static_cast<float>(Config::GetInstance().windowsWidth);
}

Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Control::Init();

    m_background = new SpriteNode("../assets/Sprites/rect.png");
    m_background->m_spriteDisplayMode = Fit;
    m_background->m_transform.SetSize(m_windowWidth, m_windowHeight);
    m_background->SetRGBA(0,0,0,1);
    AddChild(*m_background);

    m_aut = new SpriteNode("../assets/Splash/aut.png");
    m_aut->SetRGBA(1,1,1,0.01);
    m_aut->m_globalTransformationFlag = Disable;
    m_aut->m_globalTransform.SetScale(1.7);
    m_screens.push_back(m_aut);
    AddChild(*m_aut);

    m_kiwi = new SpriteNode("../assets/Splash/kiwi.png");
    m_kiwi->SetRGBA(1,1,1,0.01);
    m_kiwi->m_globalTransformationFlag = Disable;
    m_kiwi->m_transform.SetScale(0.5);
    m_screens.push_back(m_kiwi);
    AddChild(*m_kiwi);
}

void Splashscreen::Process(float deltaTime) {
    Control::Process(deltaTime);

    if (m_iCurrentScreen >= m_screens.size()) {
        SceneManager::GetInstance().LoadScene("MainMenu");
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

    m_aut->m_globalTransform.position.x = m_windowWidth / 2 - m_aut->m_globalTransform.GetWidth() / 2;
    m_aut->m_globalTransform.position.y = m_windowHeight / 2 - m_aut->m_globalTransform.GetHeight() / 2;
    m_kiwi->m_globalTransform.position.x = m_windowWidth / 2 - m_kiwi->m_globalTransform.GetWidth() / 2;
    m_kiwi->m_globalTransform.position.y = m_windowHeight / 2 - m_kiwi->m_globalTransform.GetHeight() / 2;
}
