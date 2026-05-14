
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
    TextControl* m_timerDisplay;
    Player* m_player;
    AnimatedSpriteNode * m_dropOffLocationSprite;
    SpriteNode * m_pickedObject;
    SpriteNode * m_unpickdObject;
    std::vector<Enemy*> m_enemies;
    FMOD::Channel* m_backgroundSound;
    FMOD::Channel* m_pickupSound;
};
REGISTER_CLASS(Level1)


#endif //GP_FRAMEWORK_LEVEL1_H