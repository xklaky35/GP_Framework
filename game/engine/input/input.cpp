#include "input.h"

#include "../logmanager/logmanager.h"

namespace Engine {

    InputManager::InputManager() : m_bRelativeMouseMode(false)
                                   , m_pXboxController(nullptr), m_iNumAttachedControllers(0),
                                   m_previousKey(SDLK_UNKNOWN) {
    }

    InputManager::~InputManager() = default;

    InputManager* InputManager::m_pInstance = nullptr;

    InputManager & InputManager::GetInstance() {
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
        if (event.key.type == SDL_KEYDOWN) {
            if (m_pressedKeys[event.key.keysym.sym] == BS_NEUTRAL) {
                m_pressedKeys[event.key.keysym.sym] = BS_PRESSED;
                m_isInInstableState.push(KeyState{event.key.keysym.sym, BS_PRESSED});
            }
            m_previousKey = event.key.keysym.sym;
        }

        if (event.key.type == SDL_KEYUP) {
            m_pressedKeys[event.key.keysym.sym] = BS_RELEASED;
            m_isInInstableState.push(KeyState{event.key.keysym.sym, BS_RELEASED});
            m_previousKey = event.key.keysym.sym;
        }


        // mouse events
        if (event.type == SDL_MOUSEBUTTONUP) {
            m_currentMouseEvent = event.button;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            m_currentMouseEvent = event.button;
        }
        else {
            m_currentMouseEvent = SDL_MouseButtonEvent();
        }

        if (event.type == SDL_MOUSEMOTION) {
            m_mousePosition.x = static_cast<float>(event.motion.x);
            m_mousePosition.y = static_cast<float>(event.motion.y);
        }
    }


    ButtonState InputManager::GetButtonState(SDL_Keycode keyCode) {
        return m_pressedKeys[keyCode];
    }


    Vector2d InputManager::GetMousePosition() const {
        return m_mousePosition ;
    }

    SDL_MouseButtonEvent InputManager::GetCurrentMouseEvent() const {
        return m_currentMouseEvent;
    }

    void InputManager::Process(float delta_time) {

        while (!m_isInInstableState.empty()) {
            if (m_isInInstableState.front().state == BS_PRESSED) {
                m_pressedKeys[m_isInInstableState.front().code] = BS_HELD;
            }

            if (m_isInInstableState.front().state == BS_RELEASED) {
                m_pressedKeys[m_isInInstableState.front().code] = BS_NEUTRAL;
            }

            m_isInInstableState.pop();
        }


    }
}

