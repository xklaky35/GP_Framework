#ifndef GP_FRAMEWORK_LEVEL1_H
#define GP_FRAMEWORK_LEVEL1_H

#include "../enemy.h"
#include "../player.h"
#include "../../../engine/nodes/node.h"
#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/nodes/ui/textcontrol.h"


using namespace Engine;
class Level1 : public Node {

public:
    Level1();
    ~Level1() override;
    void Init() override;
    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

private:
    TextControl* m_pTimerDisplay;
    Player* m_pPlayer;
    AnimatedSpriteNode * m_pDropOffLocationSprite;
    SpriteNode * m_pPickedObject;
    SpriteNode * m_pUnpickdObject;
    std::vector<Enemy*> m_enemies;
    FMOD::Channel* m_pBackgroundSound;
    FMOD::Channel* m_pPickupSound;
};
REGISTER_CLASS(Level1)


#endif //GP_FRAMEWORK_LEVEL1_H