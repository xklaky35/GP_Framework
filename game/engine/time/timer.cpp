//
// Created by leon on 07.05.26.
//

#include "timer.h"

Timer* Timer::m_pInstance = nullptr;
Timer &Timer::GetInstance() {
    if (m_pInstance == nullptr) {
        m_pInstance = new Timer();
    }
    return *m_pInstance;
}

void Timer::DestroyInstance() {
    if (m_pInstance != nullptr) {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void Timer::Tick() {
    auto now = Clock::now();
    m_DeltaTime = std::chrono::duration<float>(now - m_LastTime).count();
    m_TotalTime += m_DeltaTime;
    m_LastTime = now;
    ++m_FrameCount;
}

void Timer::Reset() {
    m_StartTime = Clock::now();
    m_LastTime = m_StartTime;
    m_DeltaTime = 0.0f;
    m_TotalTime = 0.0f;
    m_FrameCount = 0;
}

float Timer::GetTotalTime() const {
    return m_TotalTime;
}
