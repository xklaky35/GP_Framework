#ifndef GP_FRAMEWORK_SCENESELECTOR_H
#define GP_FRAMEWORK_SCENESELECTOR_H

#include "../imgui/imguiwindowbaseclass.h"

namespace Engine {

    class SceneSelector : public ImGuiWindowBaseClass {
    public:
        SceneSelector(bool visible);
        void DrawDebug() override;
    };

}

#endif //GP_FRAMEWORK_SCENESELECTOR_H