#include "../../config/config.h"
#include "meteorgenerator.h"
#include "meteors.h"

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
    m->SetVelocity(static_cast<float>(GetRandomNumber(5,100)));
    m->SetSpinSpeed(static_cast<float>(GetRandomNumber(1, 20)));
    m->SetSpinDirection(GetRandomNumber(0, 1) == 0 ? 1 : -1);
    m->m_globalTransform.position.x = static_cast<float>(GetRandomNumber(0, Config::GetInstance().windowsWidth));
    m->m_globalTransform.SetScale(static_cast<float>(GetRandomNumber(1, 3)) / 10);
    AddChild(*m);

    m_meteorCount++;
}


int MeteorGenerator::GetRandomNumber(int min, int max) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min, max);
    return dis(gen);
}
