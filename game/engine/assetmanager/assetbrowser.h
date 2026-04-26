#ifndef GP_FRAMEWORK_ASSETBROWSER_H
#define GP_FRAMEWORK_ASSETBROWSER_H

#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

#include "imgui.h"
#include "../imgui/imguiwindowbaseclass.h"


namespace Engine {
    struct AssetField;

    class AssetBrowser : public ImGuiWindowBaseClass {
    public:

        AssetBrowser(bool visible);

        void DrawDebug() override;

        void AddItems(int count);
        void ClearItems();
        void UpdateLayoutSizes(float avail_width);
        void Draw(const char *title, bool *p_open);


    public:
        bool isVisible = false; // wether the ImGui window is visible from the beginning

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
        ImVector<AssetField> Items;               // Our items
        ImGuiSelectionBasicStorage Selection;     // Our selection (ImGuiSelectionBasicStorage + helper funcs to handle deletion)
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
