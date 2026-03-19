//
// Created by leon on 17.03.26.
//

#include "mainmenu.h"
#include "../../config/config.h"
#include "../../engine/input/input.h"
#include "../../engine/scenemanager/scenemanager.h"

MainMenu::MainMenu() : m_bSelectionLocked(false), m_currentSelection(0), m_sceneOptions{"Astroids","SpaceInvaders","Robotron","Quit"} {
    vcontainer1 = VContainer();
    h1 = HContainer();
    h2 = HContainer();
    h3 = HContainer();
    h4 = HContainer();
    t1 = TextControl();
    t2 = TextControl();
    t3 = TextControl();
    t4 = TextControl();
    spacer = Control();
    cursor = TextControl();

}

MainMenu::~MainMenu() {

}

void MainMenu::Init() {
    HContainer::Init();


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
    spacer.m_containerSizing.m_bExpandHorizontal = true;
    spacer.m_containerSizing.m_bExpandVertical = true;

    vcontainer1.m_containerSizing.m_bExpandHorizontal = true;
    vcontainer1.m_containerSizing.m_bExpandVertical = true;


    // CENTER LAYOUT

    h1.m_containerSizing.m_bExpandHorizontal = true;
    h1.m_containerSizing.m_bExpandVertical = true;
    h2.m_containerSizing.m_bExpandHorizontal = true;
    h2.m_containerSizing.m_bExpandVertical = true;
    h3.m_containerSizing.m_bExpandHorizontal = true;
    h3.m_containerSizing.m_bExpandVertical = true;
    h4.m_containerSizing.m_bExpandHorizontal = true;
    h4.m_containerSizing.m_bExpandVertical = true;


    // ROW LAYOUT

    t1.m_containerSizing.m_bExpandHorizontal = true;
    t1.m_containerSizing.m_bExpandVertical = true;
    t2.m_containerSizing.m_bExpandHorizontal = true;
    t2.m_containerSizing.m_bExpandVertical = true;
    t3.m_containerSizing.m_bExpandHorizontal = true;
    t3.m_containerSizing.m_bExpandVertical = true;
    t4.m_containerSizing.m_bExpandHorizontal = true;
    t4.m_containerSizing.m_bExpandVertical = true;

    t1.m_containerSizing.m_verticalBehavior = v_Center;
    t2.m_containerSizing.m_verticalBehavior = v_Center;
    t3.m_containerSizing.m_verticalBehavior = v_Center;
    t4.m_containerSizing.m_verticalBehavior = v_Center;
    t1.m_containerSizing.m_horizontalBehavior = h_Center;
    t2.m_containerSizing.m_horizontalBehavior = h_Center;
    t3.m_containerSizing.m_horizontalBehavior = h_Center;
    t4.m_containerSizing.m_horizontalBehavior = h_Center;

    t1.SetText("Play Astroids");
    t2.SetText("Play Space Invaders");
    t3.SetText("Play Robotron");
    t4.SetText("Quit");


    cursor.m_containerSizing.m_bExpandVertical = true;
    cursor.m_containerSizing.m_verticalBehavior = v_Center;
    cursor.SetText(">");


    // Add Text to rows
    m_selectionParents[0] = &h1;
    h1.AddChild(cursor);
    h1.AddChild(t1);
    m_selectionParents[1] = &h2;
    h2.AddChild(cursor);
    h2.AddChild(t2);
    m_selectionParents[2] = &h3;
    h3.AddChild(cursor);
    h3.AddChild(t3);
    m_selectionParents[3] = &h4;
    h4.AddChild(cursor);
    h4.AddChild(t4);


    // Add rows to center column
    vcontainer1.AddChild(spacer);
    vcontainer1.AddChild(h1);
    vcontainer1.AddChild(h2);
    vcontainer1.AddChild(h3);
    vcontainer1.AddChild(h4);
    vcontainer1.AddChild(spacer);


    AddChild(spacer);
    AddChild(vcontainer1);
    AddChild(spacer);
}


void MainMenu::Process(float deltaTime) {
    HContainer::Process(deltaTime);

    if (Input::GetCurrentEvents().IsPressed(SDLK_DOWN)) {
        if (!m_bSelectionLocked) {
            if (m_currentSelection < 3) m_currentSelection++;
            m_bSelectionLocked = true;
        }
    }
    else if (Input::GetCurrentEvents().IsPressed(SDLK_UP)) {
        if (!m_bSelectionLocked) {
            if (m_currentSelection > 0) m_currentSelection--;
            m_bSelectionLocked = true;
        }
    }
    else if (Input::GetCurrentEvents().IsPressed(SDLK_RETURN)) {
        if (!m_bSelectionLocked) {
            SceneManager::GetInstance().LoadScene(m_sceneOptions[m_currentSelection]);
            //return;
        }
    }
    else {
        m_bSelectionLocked = false;
    }
    cursor.m_parent = m_selectionParents[m_currentSelection];
}


void MainMenu::Draw(Renderer &renderer) {
    HContainer::Draw(renderer);

}
