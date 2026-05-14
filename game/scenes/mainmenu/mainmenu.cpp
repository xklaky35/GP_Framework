#include "mainmenu.h"
#include "../../config/config.h"
#include "../../engine/game.h"
#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/scenemanager/scenemanager.h"
#include "../../engine/sound/soundmanager.h"
#include "../../engine/time/timer.h"

MainMenu::MainMenu() : m_currentSelection(0), m_backgroundMusic(nullptr){
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
    SetupNode("MainMenu", NT_Custom);
}

MainMenu::~MainMenu() {
    if (m_backgroundMusic != nullptr) {
        m_backgroundMusic->stop();
    }
}

void MainMenu::Init() {
    Timer::GetInstance().Reset();
    HContainer::Init();
    m_backgroundMusic = SoundManager::GetInstance().PlaySound("menuBackground.mp3");
    if (m_backgroundMusic != nullptr) {
        m_backgroundMusic->setMode(FMOD_LOOP_NORMAL);
        m_backgroundMusic->setVolume(8);
    }

    /* The Main menu proveds a choice between the 3 different games to play
     * The Games are interactivaly selectable by moving the cursor up and down and pressing enter once a game is selected
     *
     * >  Play SpaceInvaders
     *    Play Robotron
     *    Play Asteroids
     *
     *         Quit
     */

    // OUTER LAYOUT
    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
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


    t1->SetText("Play");
    t1->SetPointSize(50);
    t2->SetText("Quit");
    t2->SetPointSize(50);
    t3->SetText("GRAPPLE");
    t3->SetPointSize(70);


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

    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/mainmenu/MainMenu.ini");
}


void MainMenu::Process(float deltaTime) {
    HContainer::Process(deltaTime);
    HandleMouseInteraction();

    if (t1->m_textSprite != nullptr && t2->m_textSprite != nullptr) {
        t1->m_textSprite->m_iLayer = 1;
        t2->m_textSprite->m_iLayer = 1;
        t3->m_textSprite->m_iLayer = 1;
    }
}

void MainMenu::SetupParameter(IniParser *parser, const std::string &sectionId) {
    HContainer::SetupParameter(parser, sectionId);
}

void MainMenu::Draw(Renderer &renderer) {
    HContainer::Draw(renderer);
}

void MainMenu::HandleMouseInteraction() {


    auto mousePos = InputManager::GetInstance().GetMousePosition();
    auto mouseEvent = InputManager::GetInstance().GetCurrentMouseEvent();

    if (mousePos.y < h2->GetGlobalPosition().y && m_currentSelection != 0) {
        t1->SetTextRGBA(1,1,1,255);
        t2->SetTextRGBA(0.5,0.5,0.5,255);
        m_currentSelection = 0;
        SoundManager::GetInstance().PlaySound("menuHover.wav");
    }
    if (mousePos.y > h2->GetGlobalPosition().y && m_currentSelection != 1) {
        t1->SetTextRGBA(0.5,0.5,0.5,255);
        t2->SetTextRGBA(1,1,1,255);
        m_currentSelection = 1;
        SoundManager::GetInstance().PlaySound("menuHover.wav");
    }
    if (mouseEvent.type == SDL_MOUSEBUTTONUP && mouseEvent.button == 1) {
        ExecuteSelection();
    }
}

void MainMenu::ExecuteSelection() const {
    auto currentTime = Timer::GetInstance().GetTotalTime();
    if (currentTime < 0.5f) return;

    if (m_currentSelection == 0)
        SceneManager::GetInstance().SetSceneActive("Whoosh");
    if (m_currentSelection == 1)
        Game::GetInstance().Quit();
    SoundManager::GetInstance().PlaySound("selectOption.mp3");
}
