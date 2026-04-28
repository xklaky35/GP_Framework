#ifndef GP_FRAMEWORK_ASSETBROWSER_H
#define GP_FRAMEWORK_ASSETBROWSER_H

#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

#include "imgui.h"
#include "imgui_internal.h"
#include "../imgui/imguiwindowbaseclass.h"
#include "../nodes/node.h"
#include "../nodes/nodefactory.h"


namespace Engine {


    enum AssetType {
        AT_Node = 1,
        AT_Component
    };

    struct AssetField {
        ImGuiID ID;
        AssetType Type;
        bool IsSelected;
        std::string AssetName;
        std::string AssetPath;

        Node *node;

        AssetField(const AssetType type, std::string assetPath) : ID(0), Type(type), IsSelected(false),
                                                                  AssetPath(std::move(assetPath)), node(nullptr) {
            std::string pathCpy = AssetPath;
            int pos = 0;
            std::string token;
            while ((pos = pathCpy.find('/')) != std::string::npos) {
                token = pathCpy.substr(0, pos);
                pathCpy.erase(0, pos + 1);
            }
            AssetName = pathCpy; // everything exept the last part of the path got deleted
        }

        AssetField(const AssetType type, Node *node) : ID(0), Type(type), IsSelected(false),
                                                       AssetName(node->m_name), node(node) {
        }

        Node *GetNode() {
            if (!AssetPath.empty())
                NodeFactory::GetInstance().InitWithConfiguration(node, AssetPath);

            return node;
        }

        static const ImGuiTableSortSpecs *s_current_sort_specs;

        static void SortWithSortSpecs(ImGuiTableSortSpecs *sort_specs, AssetField *items, int items_count) {
            s_current_sort_specs = sort_specs; // Store in variable accessible by the sort function.
            if (items_count > 1)
                qsort(items, (size_t) items_count, sizeof(items[0]), CompareWithSortSpecs);
            s_current_sort_specs = nullptr;
        }

        // Compare function to be used by qsort()
        static int IMGUI_CDECL CompareWithSortSpecs(const void *lhs, const void *rhs) {
            const auto *a = static_cast<const AssetField *>(lhs);
            const auto *b = static_cast<const AssetField *>(rhs);
            for (int n = 0; n < s_current_sort_specs->SpecsCount; n++) {
                const ImGuiTableColumnSortSpecs *sort_spec = &s_current_sort_specs->Specs[n];
                int delta = 0;
                if (sort_spec->ColumnIndex == 0)
                    delta = ((int) a->ID - (int) b->ID);
                else if (sort_spec->ColumnIndex == 1)
                    delta = (a->Type - b->Type);
                if (delta > 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
                if (delta < 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
            }
            return (int) a->ID - (int) b->ID;
        }
    };


    class AssetBrowser : public ImGuiWindowBaseClass {
    public:
        AssetBrowser(bool visible);

        void DrawDebug() override;

        void AddItem(AssetField assetName);
        void ClearItems();
        void UpdateLayoutSizes(float avail_width);
        void Draw(const char *title, bool *p_open);


    public:
        bool isVisible = false; // wether the ImGui window is visible from the beginning
        std::vector<AssetField> Items;              // Our items

    private:

        // Options
        bool            ShowTypeOverlay = true;
        bool            AllowSorting = true;
        bool            AllowDragUnselected = false;
        bool            AllowBoxSelect = true;
        float           IconSize = 32.0f;
        int             IconSpacing = 10;
        int             IconHitSpacing = 4;         // Increase hit-spacing if you want to make it possible to clear or box-select from gaps. Some spacing is required to able to amend with Shift+box-select. Value is small in Explorer.
        bool            StretchSpacing = true;

        // State
        ImGuiSelectionBasicStorage Selection;       // Our selection
        ImGuiID         NextItemId = 0;             // Unique identifier when creating new items
        bool            RequestDelete = false;      // Deferred deletion request
        bool            RequestSort = false;        // Deferred sort request
        float           ZoomWheelAccum = 0.0f;      // Mouse wheel accumulator to handle smooth wheels better

        // Calculated sizes for layout, output of UpdateLayoutSizes(). Could be locals but our code is simpler this way.
        ImVec2          LayoutItemSize;
        ImVec2          LayoutItemStep;             // == LayoutItemSize + LayoutItemSpacing
        float           LayoutItemSpacing = 0.0f;
        float           LayoutSelectableSpacing = 0.0f;
        float           LayoutOuterPadding = 0.0f;
        int             LayoutColumnCount = 0;
        int             LayoutLineCount = 0;
    };


}

#endif
