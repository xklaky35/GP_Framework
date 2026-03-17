//
// Created by leon on 17.03.26.
//

#include "mainmenu.h"
#include "../../config/config.h"

MainMenu::MainMenu() {
    testBox1 = Control(true);
    testBox2 = Control(true);
    testBox3 = Control(true);
}

MainMenu::~MainMenu() {

}

void MainMenu::Init() {
    VContainer::Init();

    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);

    testBox1.m_containerSizing.m_bExpandHorizontal = true;
    testBox1.m_containerSizing.m_bExpandVertical = true;
    testBox1.m_containerSizing.m_horizontalBehavior = h_Fill;
    testBox1.m_containerSizing.m_verticalBehavior = v_Fill;
    testBox1.m_positionMode = Position;
    AddChild(testBox1);


    testBox2.m_containerSizing.m_bExpandHorizontal = true;
    testBox2.m_containerSizing.m_bExpandVertical = true;
    testBox2.m_containerSizing.m_horizontalBehavior = h_Fill;
    testBox2.m_containerSizing.m_verticalBehavior = v_Fill;
    testBox2.m_positionMode = Position;
    AddChild(testBox2);

    testBox3.m_containerSizing.m_bExpandHorizontal = true;
    testBox3.m_containerSizing.m_bExpandVertical = true;
    testBox3.m_containerSizing.m_horizontalBehavior = h_Fill;
    testBox3.m_containerSizing.m_verticalBehavior = v_Fill;
    testBox3.m_positionMode = Position;
    AddChild(testBox3);

}

void MainMenu::Draw(Renderer &renderer) {
    VContainer::Draw(renderer);
}
