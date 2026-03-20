#include "input.h"

namespace Engine {

    InputManager* InputManager::m_pInstance = nullptr;

    InputManager & InputManager::GetCurrentEvents() {
        if (m_pInstance == nullptr) {
            m_pInstance = new InputManager();
        }
        return *m_pInstance;
    }
    void InputManager::DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void InputManager::RegisterEvent(SDL_Event& event) {
        if (event.key.type == SDL_KEYDOWN ) {
            m_pressedKeys[event.key.keysym.sym] = true;
        }
        if (event.key.type == SDL_KEYUP ) {
            m_pressedKeys[event.key.keysym.sym] = false;
        }

        m_pCurrentEvent = &event;
    }


    bool InputManager::GetButtonState(SDL_Keycode keyCode) {

        return m_pressedKeys[keyCode];
    }


    InputManager::InputManager() : m_pCurrentEvent(nullptr)
        , m_previousMouseButtons(0)
        , m_currentMouseButtons(0)
        , m_bRelativeMouseMode(false) {}
    InputManager::~InputManager() = default;

}

