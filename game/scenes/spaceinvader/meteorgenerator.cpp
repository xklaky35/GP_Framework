#include "../../config/config.h"
#include "meteorgenerator.h"
#include "meteors.h"
#include "../../helper/inlinehelper.h"

#include <random>

MeteorGenerator::MeteorGenerator(int mMaxCount) : Node("Meteorgenerator"), m_meteorCount(0), m_maxMeteorCount(mMaxCount) {}


void MeteorGenerator::Process(float deltaTime) {
    Node::Process(deltaTime);
    if (m_meteorCount < m_maxMeteorCount) {
        GenerateMetheor();
    }
}


void MeteorGenerator::GenerateMetheor() {
    Meteors *m = new Meteors();
    m->SetSpinSpeed(static_cast<float>(GetRandomInt(1, 20)));
    m->SetSpinDirection(GetRandomPosOrNeg());
    m->m_globalTransform.position.x = static_cast<float>(GetRandomInt(0, Config::GetInstance().windowsWidth));
    m->m_globalTransform.position.y = static_cast<float>(GetRandomInt(0, Config::GetInstance().windowsWidth));
    m->m_globalTransform.SetScale(static_cast<float>(GetRandomInt(1, 3)) / 10);
    AddChild(*m);

    m_meteorCount++;
}