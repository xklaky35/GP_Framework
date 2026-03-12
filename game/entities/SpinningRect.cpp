#include "SpinningRect.h"


SpinningRect::SpinningRect(float spinSpeed, float height, float width, const char* path) : Spritenode(height, width, path),  m_spinSpeed(spinSpeed) {
}

void SpinningRect::Process(float deltaTime) {
    Spritenode::Process(deltaTime);
    Spin(deltaTime);
}


void SpinningRect::Spin(float deltaTime) {
    m_transform->rotation += m_spinSpeed * deltaTime;

}
