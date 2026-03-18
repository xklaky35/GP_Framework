//
// Created by leon on 17.03.26.
//

#include "mainmenu.h"
#include "../../config/config.h"

MainMenu::MainMenu() {
    testBox1 = Control(true);
    testBox2 = Control(true);
    testBox3 = Control(true);
    testBox4 = Control(true);
    testBox5 = Control(true);
    vcontainer1 = VContainer();
    hcontainer2 = HContainer();
    mcontainer1 = MarginContainer();
}

MainMenu::~MainMenu() {

}

void MainMenu::Init() {
    VContainer::Init();

    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);
    m_controlSpace.x = m_globalTransform.GetWidth();
    m_controlSpace.y = m_globalTransform.GetHeight();



    vcontainer1.AddChild(testBox1);
    vcontainer1.AddChild(testBox4);
    hcontainer2.AddChild(testBox2);
    hcontainer2.AddChild(testBox5);

    mcontainer1.AddChild(vcontainer1);

    AddChild(mcontainer1);
    AddChild(hcontainer2);
    AddChild(testBox3);

}

void MainMenu::Draw(Renderer &renderer) {
    VContainer::Draw(renderer);
}
