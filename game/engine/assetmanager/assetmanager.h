#ifndef GP_FRAMEWORK_ASSETMANAGER_H
#define GP_FRAMEWORK_ASSETMANAGER_H

#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

#include <vector>
#include "assetbrowser.h"


class ImGuiWindowBaseClass;

namespace Engine {




    class AssetManager {
    public:
        static AssetManager& GetInstance();
        static void DeleteInstance();

        bool Initialise();

        bool LoadAssets(AssetBrowser *assetBrowser);

        void DrawDebug() const;


    private:
        AssetManager();
        ~AssetManager();

    protected:
        static AssetManager *m_pInstance;

    public:
        std::vector<AssetField>* assets;
    private:
        std::vector<ImGuiWindowBaseClass*> windows;
    };


}

#endif