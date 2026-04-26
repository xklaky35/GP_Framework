#ifndef GP_FRAMEWORK_IMGUIWINDOWBASECLASS_H
#define GP_FRAMEWORK_IMGUIWINDOWBASECLASS_H
#include <string>


class ImGuiWindowBaseClass {
public:
    virtual ~ImGuiWindowBaseClass() = default;
    virtual void DrawDebug() = 0;

public:
    bool visible;
    std::string name;
};


#endif //GP_FRAMEWORK_IMGUIWINDOWBASECLASS_H