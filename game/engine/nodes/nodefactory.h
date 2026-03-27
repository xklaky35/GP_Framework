
#ifndef GP_FRAMEWORK_NODEFACTORY_H
#define GP_FRAMEWORK_NODEFACTORY_H
#include "node.h"
#include "spritenode.h"

namespace Engine {
    class NodeFactory {
    public:
        static NodeFactory &GetInstance();
        static void DestroyInstance();

        void AttachChildren(Node*);

    private:
        static void SetupNodeParameter(Node *n, IniParser *parser, std::string sectionName);
        static void SetupSpriteParameter(SpriteNode *spriteNode, IniParser *parser, std::string section);

        static int GetIndexOf(const char*[], const char*, int length);


    private:
        NodeFactory();
        ~NodeFactory();

        static NodeFactory *m_pInstance;

    };


}
#endif //GP_FRAMEWORK_NODEFACTORY_H