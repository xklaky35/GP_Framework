//
// Created by leon on 26.03.26.
//

#include "nodefactory.h"

#include <cassert>

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

    void NodeFactory::AttachChildren(Node* n) {
        assert(n->m_iniParser);
        IniParser* parser = n->m_iniParser;
        for (const auto& [section, data] : n->m_iniParser->m_data) {
            if (section == n->m_name) {
                SetupNodeParameter(n, parser, n->m_name);
            }
            if (section == "SpriteNode") {
                auto* spriteNode = new SpriteNode();
                SetupNodeParameter(spriteNode, parser, section);
                SetupSpriteParameter(spriteNode, parser, section);
                n->AddChild(*spriteNode);
            }
            if (section== "AnimatedSpriteNode") {
                auto* animatedSprite = new AnimatedSpriteNode();
                SetupNodeParameter(animatedSprite, parser, section);
                SetupSpriteParameter(animatedSprite, parser, section);
                animatedSprite->m_bIsLooping = parser->GetValueAsBoolean(section, "isLooping");
                animatedSprite->m_bIsAnimating = parser->GetValueAsBoolean(section, "isAnimating");
                animatedSprite->m_fFrameDuration = parser->GetValueAsFloat(section, "frameDuration");
                animatedSprite->m_frameWidth = parser->GetValueAsFloat(section, "frameWidth");
                animatedSprite->m_frameHeight = parser->GetValueAsFloat(section, "frameHeight");
                n->AddChild(*animatedSprite);
            }
        }
    }

    void NodeFactory::SetupNodeParameter(Node* n, IniParser* parser, std::string sectionName) {
        n->m_globalTransformationFlag = static_cast<InheritanceFlag>(GetIndexOf(InheritanceFlagStrings, parser->GetValueAsString(sectionName, "inheritanceFlag").c_str(), 3));
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

    void NodeFactory::SetupSpriteParameter(SpriteNode* spriteNode, IniParser* parser, std::string section) {
        spriteNode->m_spriteDisplayMode = static_cast<SpriteDisplayFlag>(GetIndexOf(
            SpriteDisplayFlagStrings, parser->GetValueAsString(section, "spriteDisplayMode").c_str(), 3));
        spriteNode->m_iLayer = parser->GetValueAsInt(section, "layer");
        spriteNode->SetRGBA(
            parser->GetValueAsFloat(section, "redTint"),
            parser->GetValueAsFloat(section, "blueTint"),
            parser->GetValueAsFloat(section, "greenTint"),
            parser->GetValueAsFloat(section, "alpha")
        );

        spriteNode->SetSpritePath(parser->GetValueAsString(section, "spritePath"));
        spriteNode->m_iniParser = parser;
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

