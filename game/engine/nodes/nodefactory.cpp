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
        n->m_dataFilePath = datafilePath;
        n->m_iniParser->LoadIniFile(datafilePath);
        ConfigureNode(n);
    }


    void NodeFactory::ConfigureNode(Node* n) {
        for (const auto& [section, data] : n->m_iniParser->m_data) {

            std::string nodeType = n->m_iniParser->GetValueAsString(section, "nodeType");

            Node* nodeToConfigure = n;

            // check wether the main node is being configured or if child nodes need to be added
            bool configureMainNode = n->m_iniParser->GetValueAsString(section, "name") == n->m_name;


            // create child nodes
            if (nodeType == NodeTypeStrings[NT_Node]) {
                if (!configureMainNode)
                    nodeToConfigure = new Node();

                SetupNodeParameter(nodeToConfigure, n->m_iniParser, section);

                if (!configureMainNode)
                    n->AddChild(*nodeToConfigure);
            }
            if (nodeType == NodeTypeStrings[NT_SpriteNode]) {
                if (!configureMainNode)
                    nodeToConfigure = new SpriteNode();
                SetupNodeParameter(nodeToConfigure, n->m_iniParser, section);
                SetupSpriteParameter(dynamic_cast<SpriteNode *>(nodeToConfigure) , n->m_iniParser, section);
                if (!configureMainNode)
                    n->AddChild(*nodeToConfigure);
            }
            if (nodeType == NodeTypeStrings[NT_AnimatedSpriteNode]) {
                if (!configureMainNode)
                    nodeToConfigure = new AnimatedSpriteNode();
                SetupNodeParameter(nodeToConfigure, n->m_iniParser, section);
                SetupSpriteParameter(dynamic_cast<SpriteNode *>(nodeToConfigure), n->m_iniParser, section);
                SetupAnimatedSpriteParameter(dynamic_cast<AnimatedSpriteNode *>(nodeToConfigure), n->m_iniParser, section);
                if (!configureMainNode)
                    n->AddChild(*nodeToConfigure);
            }
        }
    }


    void NodeFactory::SetupNodeParameter(Node* n, IniParser* parser, const std::string& sectionName) {
        n->m_UId = sectionName;
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

    void NodeFactory::SetupAnimatedSpriteParameter(AnimatedSpriteNode* n, IniParser* parser, const std::string& section) {
        n->m_bIsLooping = parser->GetValueAsBoolean(section, "isLooping");
        n->m_bIsAnimating = parser->GetValueAsBoolean(section, "isAnimating");
        n->m_fFrameDuration = parser->GetValueAsFloat(section, "frameDuration");
        n->m_frameWidth = parser->GetValueAsInt(section, "frameWidth");
        n->m_frameHeight = parser->GetValueAsInt(section, "frameHeight");
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

