#include "winscreen.h"

#include "../../engine/game.h"
#include "../../config/config.h"
#include "../../engine/time/timer.h"
#include "../../engine/input/input.h"
#include "../../engine/scenemanager/scenemanager.h"
#include "../../engine/sound/soundmanager.h"

using namespace Engine;

WinScreen::WinScreen()
    : m_iCurrentSelection(0) {

    SetupNode("WinScreen", NT_Custom);
    vcontainer1 = new VContainer();
    h1 = new HContainer();
    h2 = new HContainer();
    h3 = new HContainer();
    h4 = new HContainer();
    t1 = new TextControl();
    t2 = new TextControl();
    t3 = new TextControl();
    spacer1 = new Control();
    spacer2 = new Control();
    spacer3 = new Control();
    spacer4 = new Control();
}

void WinScreen::Init() {
    HContainer::Init();

    auto sound = SoundManager::GetInstance().PlaySound("winning.mp3");
    if (sound != nullptr) {
        sound->setVolume(3);
    }
    m_completionTime = "Completed in  " + Timer::GetInstance().GetTotalTimeAsString();

    // OUTER LAYOUT
    m_screenSize = Vector2d(static_cast<float>(Config::GetInstance().windowsWidth), static_cast<float>(Config::GetInstance().windowsHeight));
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);
    m_controlSpace.x = m_globalTransform.GetWidth();
    m_controlSpace.y = m_globalTransform.GetHeight();
    m_containerSizing.m_bExpandHorizontal = true;
    m_containerSizing.m_bExpandVertical = true;


    // Spacer config
    spacer1->m_containerSizing.m_bExpandHorizontal = true;
    spacer1->m_containerSizing.m_bExpandVertical = true;
    spacer2->m_containerSizing.m_bExpandHorizontal = true;
    spacer2->m_containerSizing.m_bExpandVertical = true;
    spacer3->m_containerSizing.m_bExpandHorizontal = true;
    spacer3->m_containerSizing.m_bExpandVertical = true;
    spacer4->m_containerSizing.m_bExpandHorizontal = true;
    spacer4->m_containerSizing.m_bExpandVertical = true;

    vcontainer1->m_containerSizing.m_bExpandHorizontal = true;
    vcontainer1->m_containerSizing.m_bExpandVertical = true;


    // CENTER LAYOUT

    h1->m_containerSizing.m_bExpandHorizontal = true;
    h1->m_containerSizing.m_bExpandVertical = true;
    h2->m_containerSizing.m_bExpandHorizontal = true;
    h2->m_containerSizing.m_bExpandVertical = true;
    h3->m_containerSizing.m_bExpandHorizontal = true;
    h3->m_containerSizing.m_bExpandVertical = true;
    h4->m_containerSizing.m_bExpandHorizontal = true;
    h4->m_containerSizing.m_bExpandVertical = true;


    // ROW LAYOUT

    t1->m_containerSizing.m_bExpandHorizontal = true;
    t1->m_containerSizing.m_bExpandVertical = true;
    t2->m_containerSizing.m_bExpandHorizontal = true;
    t2->m_containerSizing.m_bExpandVertical = true;
    t3->m_containerSizing.m_bExpandHorizontal = true;
    t3->m_containerSizing.m_bExpandVertical = true;



    t1->m_containerSizing.m_verticalBehavior = v_Center;
    t1->m_containerSizing.m_horizontalBehavior = h_Center;
    t2->m_containerSizing.m_verticalBehavior = v_Center;
    t2->m_containerSizing.m_horizontalBehavior = h_Center;
    t3->m_containerSizing.m_verticalBehavior = v_Center;
    t3->m_containerSizing.m_horizontalBehavior = h_Center;

    t1->SetText("Play again");
    t1->SetPointSize(50);
    t2->SetText("Return to menu");
    t2->SetPointSize(50);
    t3->SetText(m_completionTime);
    t3->SetPointSize(50);

    // Add Text to rows
    h1->AddChild(*t1);
    h2->AddChild(*t2);
    h3->AddChild(*t3);

    // Add rows to center column
    vcontainer1->AddChild(*spacer1);
    vcontainer1->AddChild(*h3);
    vcontainer1->AddChild(*h1);
    vcontainer1->AddChild(*h2);
    vcontainer1->AddChild(*spacer2);

    AddChild(*spacer3);
    AddChild(*vcontainer1);
    AddChild(*spacer4);

    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/winscreen/WinScreen.ini");
}

void WinScreen::Process(float deltaTime) {
    HContainer::Process(deltaTime);
    HandleMouseInteraction();
    if (t1->m_pTextSprite != nullptr && t2->m_pTextSprite != nullptr) {
        t1->m_pTextSprite->m_iLayer = 1;
        t2->m_pTextSprite->m_iLayer = 1;
        t3->m_pTextSprite->m_iLayer = 1;
    }

}

void WinScreen::Draw(Renderer &renderer) {
    HContainer::Draw(renderer);
}


void WinScreen::HandleMouseInteraction() {
    auto mousePos = InputManager::GetInstance().GetMousePosition();
    auto mouseEvent = InputManager::GetInstance().GetCurrentMouseEvent();

    if (mousePos.y < h2->GetGlobalPosition().y && m_iCurrentSelection != 0) {
        t1->SetTextRGBA(1,1,1,255);
        t2->SetTextRGBA(0.5,0.5,0.5,255);
        m_iCurrentSelection = 0;
    }
    if (mousePos.y > h2->GetGlobalPosition().y && m_iCurrentSelection != 1) {
        t1->SetTextRGBA(0.5,0.5,0.5,255);
        t2->SetTextRGBA(1,1,1,255);
        m_iCurrentSelection = 1;
    }
    if (mouseEvent.type == SDL_MOUSEBUTTONDOWN && mouseEvent.button == 1) {
        ExecuteSelection();
    }
}

void WinScreen::ExecuteSelection() const {
    if (m_iCurrentSelection == 0)
        SceneManager::GetInstance().SetSceneActive("Whoosh");
    if (m_iCurrentSelection == 1)
        SceneManager::GetInstance().SetSceneActive("MainMenu");
}
