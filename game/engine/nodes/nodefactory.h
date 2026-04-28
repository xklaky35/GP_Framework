
#ifndef GP_FRAMEWORK_NODEFACTORY_H
#define GP_FRAMEWORK_NODEFACTORY_H
#include "animatedspritenode.h"
#include "node.h"
#include "spritenode.h"

namespace Engine {
    class NodeFactory {
    public:
        static NodeFactory &GetInstance();
        static void DestroyInstance();
        void InitWithConfiguration(Node*, const std::string&);

        void ConfigureNode(Node* n);

    private:
        static void SetupNodeParameter(Node *n, IniParser*, const std::string& sectionName);
        static void SetupSpriteParameter(SpriteNode *spriteNode, IniParser*, const std::string& section);

        void SetupAnimatedSpriteParameter(AnimatedSpriteNode *n, IniParser *parser, const std::string &section);

        static int GetIndexOf(const char*[], const char*, int length);


    private:
        NodeFactory();
        ~NodeFactory();

        static NodeFactory *m_pInstance;

    };


}
#endif //GP_FRAMEWORK_NODEFACTORY_H