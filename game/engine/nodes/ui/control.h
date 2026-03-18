#ifndef GP_FRAMEWORK_CONTROL_H
#define GP_FRAMEWORK_CONTROL_H

#include "../../structs/transform.h"
#include "../../nodes/node.h"
#include "../../nodes/spritenode.h"


namespace Engine {

    enum PositionMode {
        Anker,    // ignore position values and get placed at a specified anker
        Position  // use the position of this node to place it on the screen
    };
    enum ContainerBehaviorHorizontal {
        h_Fill,    // extends the contents of this node to take all available horizontal space
        h_Left,    // get placed at the left of the container
        h_Center,  // get placed in the center of the container
        h_Right    // get placed on the right of the container
    };
    static const char* ContainerBehaviorHorizontalStrings[] = { "Fill", "Left", "Center", "Right"};

    enum ContainerBehaviorVertical {
        v_Fill,     // extends the contents of this node to take all available vertical space
        v_Top,      // get placed at the top of the container
        v_Center,   // get placed in the center of the container
        v_Bottom    // get placed on the bottom of the container
    };
    static const char* ContainerBehaviorVerticalStrings[] = { "Fill", "Top", "Center", "Bottom"};


    struct ContainerSizing {
        bool m_bExpandHorizontal;
        bool m_bExpandVertical;
        ContainerBehaviorHorizontal m_horizontalBehavior;
        ContainerBehaviorVertical m_verticalBehavior;
    };

    class Control : public Node {
    public:

        Control();
        Control(bool);
        ~Control() override;

        void Init() override;
        void Process(float) override;
        void SystemProcess() override;
        void Draw(Renderer&) override;
        void CalculateChildBounds() const;


        ContainerSizing m_containerSizing;
        PositionMode m_positionMode;
        bool m_bIsChildOfContainer;
        Vector2d m_initialSize;
        Vector2d m_screenSize;
        Vector2d m_controlBounds;
        Vector2d m_controlSpace;
        bool m_bUseDebugRect;

       SpriteNode* m_debugRect;

    };



}
#endif //GP_FRAMEWORK_CONTROL_H