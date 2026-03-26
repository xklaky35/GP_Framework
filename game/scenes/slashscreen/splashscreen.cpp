#include "splashscreen.h"
#include "../../config/config.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0),
                               m_iCurrentScreen(0),
                               m_background(nullptr),
                               m_autLogo(nullptr),
                               m_fmodLogo(nullptr) {
    m_windowHeight = static_cast<float>(Config::GetInstance().windowsHeight);
    m_windowWidth = static_cast<float>(Config::GetInstance().windowsWidth);
}

Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Control::Init();

    m_background = new SpriteNode();
    m_background->SetSpritePath(std::string("../assets/Sprites/rect.png").data());
    m_background->m_spriteDisplayMode = Fit;
    m_background->m_transform.SetSize(m_windowWidth, m_windowHeight);
    m_background->SetRGBA(0,0,0,1);
    AddChild(*m_background);

    m_autLogo = new SpriteNode();
    m_autLogo->SetSpritePath(std::string("../assets/Splash/aut.png").data());
    m_autLogo->SetRGBA(1,1,1,0.01);
    m_autLogo->m_globalTransformationFlag = Disable;
    m_autLogo->m_globalTransform.SetScale(1.7);
    m_screens.push_back(m_autLogo);
    AddChild(*m_autLogo);

    m_fmodLogo = new SpriteNode();
    m_fmodLogo->SetSpritePath(std::string("../assets/Splash/FMOD_Logo.png").data());
    m_fmodLogo->SetRGBA(1,1,1,0.01);
    m_fmodLogo->m_globalTransformationFlag = Disable;
    m_fmodLogo->m_transform.SetScale(0.5);
    m_screens.push_back(m_fmodLogo);
    AddChild(*m_fmodLogo);
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

    m_autLogo->m_globalTransform.position.x = m_windowWidth / 2 - m_autLogo->m_globalTransform.GetWidth() / 2;
    m_autLogo->m_globalTransform.position.y = m_windowHeight / 2 - m_autLogo->m_globalTransform.GetHeight() / 2;
    m_fmodLogo->m_globalTransform.position.x = m_windowWidth / 2 - m_fmodLogo->m_globalTransform.GetWidth() / 2;
    m_fmodLogo->m_globalTransform.position.y = m_windowHeight / 2 - m_fmodLogo->m_globalTransform.GetHeight() / 2;
}
