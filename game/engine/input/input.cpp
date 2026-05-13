#include "input.h"

#include "imgui.h"
#include "../game.h"
#include "../../config/config.h"
#include "../logmanager/logmanager.h"

namespace Engine {
    InputManager::InputManager()
        : m_bRelativeMouseMode(false),
          m_pXboxController(nullptr),
          m_iNumAttachedControllers(0),
          m_previousKey(SDLK_UNKNOWN),
          m_currentMouseEvent() {

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
                m_isInTransitionState.push(KeyState{event.key.keysym.sym, BS_PRESSED});
            }
            m_previousKey = event.key.keysym.sym;
        }

        if (event.key.type == SDL_KEYUP) {
            m_pressedKeys[event.key.keysym.sym] = BS_RELEASED;
            m_isInTransitionState.push(KeyState{event.key.keysym.sym, BS_RELEASED});
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
        auto mouseOffset = Game::GetInstance().GetMouseOffset();
        auto mousePosition = m_mousePosition + mouseOffset;
        auto cameraOffset = Vector2d{0,0};

        if (mouseOffset.x != 0 || mouseOffset.y != 0) {
            // temp solution!!!!!!!!!!!!! (assumes the player is always in the center of the camera)
            // Problem: the mouse click position is not transformed when the ortho matrix is transformed that sets the offset for the camera
            cameraOffset = Vector2d(Config::GetInstance().windowsWidth/2, Config::GetInstance().windowsHeight/2);
        }

        mousePosition -= cameraOffset;

        return mousePosition ;
    }

    SDL_MouseButtonEvent InputManager::GetCurrentMouseEvent() const {
        if (!ImGui::GetIO().WantCaptureMouse)
            return m_currentMouseEvent;
        return SDL_MouseButtonEvent{};
    }

    void InputManager::Process(float delta_time) {

        while (!m_isInTransitionState.empty()) {
            if (m_isInTransitionState.front().state == BS_PRESSED) {
                m_pressedKeys[m_isInTransitionState.front().code] = BS_HELD;
            }

            if (m_isInTransitionState.front().state == BS_RELEASED) {
                m_pressedKeys[m_isInTransitionState.front().code] = BS_NEUTRAL;
            }

            m_isInTransitionState.pop();
        }


    }
}

