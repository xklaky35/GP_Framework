#include "nodefactory.h"
#include "animatedspritenode.h"
#include "spritenode.h"

namespace Engine {
    NodeFactory* NodeFactory::m_pInstance = nullptr;

    NodeFactory & NodeFactory::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new NodeFactory();
        }
        return *m_pInstance;
    }

    void NodeFactory::DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void NodeFactory::InitWithConfiguration(Node* n, const std::string& datafilePath) {

        n->m_iniParser = new IniParser();
        n->m_iniParser->LoadIniFile(datafilePath);

        for (const auto& [section, data] : n->m_iniParser->m_data) {
            if (section == n->m_name) {
                SetupNodeParameter(n, n->m_iniParser, n->m_name);
            }
            if (section == "SpriteNode") {
                auto* spriteNode = new SpriteNode();
                SetupNodeParameter(spriteNode, n->m_iniParser, section);
                SetupSpriteParameter(spriteNode , n->m_iniParser, section);
                n->AddChild(*spriteNode);
            }
            if (section== "AnimatedSpriteNode") {
                auto* animatedSprite = new AnimatedSpriteNode();
                SetupNodeParameter(animatedSprite, n->m_iniParser, section);
                SetupSpriteParameter(animatedSprite, n->m_iniParser, section);
                animatedSprite->m_bIsLooping = n->m_iniParser->GetValueAsBoolean(section, "isLooping");
                animatedSprite->m_bIsAnimating = n->m_iniParser->GetValueAsBoolean(section, "isAnimating");
                animatedSprite->m_fFrameDuration = n->m_iniParser->GetValueAsFloat(section, "frameDuration");
                animatedSprite->m_frameWidth = n->m_iniParser->GetValueAsInt(section, "frameWidth");
                animatedSprite->m_frameHeight = n->m_iniParser->GetValueAsInt(section, "frameHeight");
                n->AddChild(*animatedSprite);
            }
        }
    }

    void NodeFactory::SetupNodeParameter(Node* n, IniParser* parser, const std::string& sectionName) {
        n->m_globalTransformationFlag = static_cast<InheritanceFlag>(
                GetIndexOf(
                        InheritanceFlagStrings, parser->GetValueAsString(sectionName, "inheritanceFlag").c_str(), INHERITANCE_FLAG_STRINGS_COUNT
                    )
            );
        n->m_globalTransform.rotation = parser->GetValueAsFloat(sectionName, "rotation");
        n->m_globalTransform.scale = parser->GetValueAsFloat(sectionName, "scale");
        n->m_globalTransform.baseSize.y = parser->GetValueAsFloat(sectionName, "baseSizeY");
        n->m_globalTransform.baseSize.x = parser->GetValueAsFloat(sectionName, "baseSizeX");
        n->m_transform.position.y = parser->GetValueAsFloat(sectionName, "localPosY");
        n->m_transform.position.x = parser->GetValueAsFloat(sectionName, "localPosX");
        n->m_globalTransform.position.y = parser->GetValueAsFloat(sectionName, "globalPosY");
        n->m_globalTransform.position.x = parser->GetValueAsFloat(sectionName, "globalPosX");
        n->m_name =  parser->GetValueAsString(sectionName, "name");
    }

    void NodeFactory::SetupSpriteParameter(SpriteNode* n, IniParser* parser, const std::string& section) {
        n->m_spriteDisplayMode = static_cast<SpriteDisplayFlag>(GetIndexOf(
            SpriteDisplayFlagStrings, parser->GetValueAsString(section, "spriteDisplayMode").c_str(), SPRITE_DISPLAY_FLAG_STRING_COUNT));
        n->m_iLayer = parser->GetValueAsInt(section, "layer");
        n->SetRGBA(
            parser->GetValueAsFloat(section, "redTint"),
            parser->GetValueAsFloat(section, "blueTint"),
            parser->GetValueAsFloat(section, "greenTint"),
            parser->GetValueAsFloat(section, "alpha")
        );

        n->SetSpritePath(parser->GetValueAsString(section, "spritePath"));
    }

    int NodeFactory::GetIndexOf(const char *arr[] , const char * str, const int length) {
        for (int i = 0; i < length; i++) {
            if (strcmp(arr[i], str) == 0) return i;
        }
        return 0;
    }


    NodeFactory::NodeFactory() = default;
    NodeFactory::~NodeFactory() = default;
}

