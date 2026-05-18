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
        AssetType assetType;
        NodeType nodeType;
        bool isSelected;
        std::string assetName;
        std::string assetPath;

        AssetField(const AssetType type, std::string assetPath)
            : ID(0),
              assetType(type),
              nodeType(NT_Node),
              isSelected(false),
              assetPath(std::move(assetPath)) {

            std::string pathCpy = assetPath;
            int pos = 0;
            while ((pos = static_cast<int>(pathCpy.find('/'))) != std::string::npos) {
                pathCpy.erase(0, pos + 1);
            }
            assetName = pathCpy; // everything exept the last part of the path got deleted

            pos = static_cast<int>(pathCpy.find('.'));
            if (pos != std::string::npos) {
                assetName = pathCpy.substr(0, pos);
            }
        }

        AssetField(const AssetType type, NodeType nodeType)
            : ID(0),
              assetType(type),
              nodeType(nodeType),
              isSelected(false) {
              assetName = NodeTypeStrings[nodeType];
        }

        [[nodiscard]] Node *GetNode() const {
            if (!assetPath.empty())
                return NodeFactory::GetInstance().CreateCustomNode(assetName, assetPath);


            return NodeFactory::GetInstance().CreateBaseNode(nodeType);
        }

        static const ImGuiTableSortSpecs *s_current_sort_specs;

        static void SortWithSortSpecs(const ImGuiTableSortSpecs *sort_specs, AssetField *items, int items_count) {
            s_current_sort_specs = sort_specs; // Store in variable accessible by the sort function.
            if (items_count > 1)
                qsort(items, static_cast<size_t>(items_count), sizeof(items[0]), CompareWithSortSpecs);
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
                    delta = (static_cast<int>(a->ID) - static_cast<int>(b->ID));
                else if (sort_spec->ColumnIndex == 1)
                    delta = (a->assetType - b->assetType);
                if (delta > 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
                if (delta < 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
            }
            return static_cast<int>(a->ID) - static_cast<int>(b->ID);
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
        bool            m_bShowTypeOverlay = true;
        bool            m_bAllowSorting = true;
        bool            m_bAllowDragUnselected = false;
        bool            m_bAllowBoxSelect = true;
        float           m_fIconSize = 32.0f;
        int             m_iIconSpacing = 10;
        int             m_iIconHitSpacing = 4;         // Increase hit-spacing if you want to make it possible to clear or box-select from gaps. Some spacing is required to able to amend with Shift+box-select. Value is small in Explorer.
        bool            m_bStretchSpacing = true;

        // State
        ImGuiSelectionBasicStorage m_selection;       // Our selection
        ImGuiID         m_nextItemId = 0;             // Unique identifier when creating new items
        bool            m_bRequestDelete = false;      // Deferred deletion request
        bool            m_bRequestSort = false;        // Deferred sort request
        float           m_fZoomWheelAccum = 0.0f;      // Mouse wheel accumulator to handle smooth wheels better

        // Calculated sizes for layout, output of UpdateLayoutSizes(). Could be locals but our code is simpler this way.
        ImVec2          m_layoutItemSize;
        ImVec2          m_layoutItemStep;             // == LayoutItemSize + LayoutItemSpacing
        float           m_fLayoutItemSpacing = 0.0f;
        float           m_fLayoutSelectableSpacing = 0.0f;
        float           m_fLayoutOuterPadding = 0.0f;
        int             m_iLayoutColumnCount = 0;
        int             m_iLayoutLineCount = 0;
    };


}

#endif
