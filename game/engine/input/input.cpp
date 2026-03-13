#include "input.h"

namespace Engine {
    Input* Input::m_pInstance = nullptr;

    Input & Input::GetCurrentEvents() {
        if (m_pInstance == nullptr) {
            m_pInstance = new Input();
        }
        return *m_pInstance;
    }
    void Input::DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void Input::RegisterEvent(SDL_Event& event) {
        if (event.key.type == SDL_KEYDOWN ) {
            m_pressedKeys[event.key.keysym.sym] = true;
        }
        if (event.key.type == SDL_KEYUP ) {
            m_pressedKeys[event.key.keysym.sym] = false;
        }

        m_pCurrentEvent = &event;
    }


    bool Input::IsPressed(SDL_Keycode keyCode) {
        return m_pressedKeys[keyCode];
    }


    Input::Input() : m_pCurrentEvent(nullptr) {}
    Input::~Input() = default;

}

