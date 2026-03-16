
#ifndef GP_FRAMEWORK_EVENT_H
#define GP_FRAMEWORK_EVENT_H
#include <functional>

#include "nodes/node.h"

namespace Engine {
    template<class X>
    class Event {

    public:
        Event() = default;
        ~Event() = default;


        template<typename T>
        void Register(void (T::*callback)(), T&element) {
            m_actionContainer.push_back(std::bind(callback, &element, std::placeholders::_1));
        }

        template<typename T>
        void Register(void (T::*callback)(const X*), T&element) {
            m_paramEventContainer.push_back(std::bind(callback, &element, std::placeholders::_1));
        }

        void Emit() const {
            for (auto f : m_actionContainer) {
                f();
            }
        }

        void Emit(X *param) const {
            for (auto f : m_paramEventContainer) {
                f(param);
            }
        }


    public:
        std::vector<std::function<void()>> m_actionContainer;
        std::vector<std::function<void(Node *)>> m_paramEventContainer;
    };
}

#endif //GP_FRAMEWORK_EVENT_H