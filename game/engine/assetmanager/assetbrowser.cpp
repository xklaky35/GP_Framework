#include "assetbrowser.h"
#include "../imgui/imguimanager.h"


namespace Engine {
    const ImGuiTableSortSpecs* AssetField::s_current_sort_specs = nullptr;

    AssetBrowser::AssetBrowser(const bool visible) {
        name = "AssetBrowser";
        this->visible = visible;
    }


    void AssetBrowser::DrawDebug() {

        bool p_open;
        Draw(name.c_str(), &p_open);

    }
    void AssetBrowser::AddItem(AssetField asset)
    {
        if (Items.empty())
            m_nextItemId = 0;


        asset.ID = m_nextItemId;
        Items.push_back(asset);
        m_nextItemId++;
        m_bRequestSort = true;
    }
    void AssetBrowser::ClearItems()
    {
        Items.clear();
        m_selection.Clear();
    }

    // Logic would be written in the main code BeginChild() and outputting to local variables.
    // We extracted it into a function so we can call it easily from multiple places.
    void AssetBrowser::UpdateLayoutSizes(float avail_width)
    {
        // Layout: when not stretching: allow extending into right-most spacing.
        m_fLayoutItemSpacing = static_cast<float>(m_iIconSpacing);
        if (m_bStretchSpacing == false)
            avail_width += floorf(m_fLayoutItemSpacing * 0.5f);

        // Layout: calculate number of icon per line and number of lines
        m_layoutItemSize = ImVec2(floorf(m_fIconSize), floorf(m_fIconSize));
        m_iLayoutColumnCount = IM_MAX((int)(avail_width / (m_layoutItemSize.x + m_fLayoutItemSpacing)), 1);
        m_iLayoutLineCount = (static_cast<int>(Items.size()) + m_iLayoutColumnCount - 1) / m_iLayoutColumnCount;

        // Layout: when stretching: allocate remaining space to more spacing. Round before division, so item_spacing may be non-integer.
        if (m_bStretchSpacing && m_iLayoutColumnCount > 1)
            m_fLayoutItemSpacing = floorf(avail_width - m_layoutItemSize.x * static_cast<float>(m_iLayoutColumnCount)) / static_cast<float>(m_iLayoutColumnCount);

        m_layoutItemStep = ImVec2(m_layoutItemSize.x + m_fLayoutItemSpacing, m_layoutItemSize.y + m_fLayoutItemSpacing);
        m_fLayoutSelectableSpacing = IM_MAX(floorf(m_fLayoutItemSpacing) - m_iIconHitSpacing, 0.0f);
        m_fLayoutOuterPadding = floorf(m_fLayoutItemSpacing * 0.5f);
    }


    void AssetBrowser::Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(m_fIconSize * 25, m_fIconSize * 15), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Separator();
                if (ImGui::MenuItem("Close", nullptr, false, p_open != nullptr))
                    if (p_open != nullptr) {
                        *p_open = false;
                    }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Options"))
            {
                ImGui::PushItemWidth(ImGui::GetFontSize() * 10);

                ImGui::SeparatorText("Contents");
                ImGui::Checkbox("Show Type Overlay", &m_bShowTypeOverlay);
                ImGui::Checkbox("Allow Sorting", &m_bAllowSorting);

                ImGui::SeparatorText("Selection Behavior");
                ImGui::Checkbox("Allow dragging unselected item", &m_bAllowDragUnselected);
                ImGui::Checkbox("Allow box-selection", &m_bAllowBoxSelect);

                ImGui::SeparatorText("Layout");
                ImGui::SliderFloat("Icon Size", &m_fIconSize, 16.0f, 128.0f, "%.0f");
                ImGui::SameLine(); ImguiManager::HelpMarker("Use Ctrl+Wheel to zoom");
                ImGui::SliderInt("Icon Spacing", &m_iIconSpacing, 0, 32);
                ImGui::SliderInt("Icon Hit Spacing", &m_iIconHitSpacing, 0, 32);
                ImGui::Checkbox("Stretch Spacing", &m_bStretchSpacing);
                ImGui::PopItemWidth();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Show a table with ONLY one header row to showcase the idea/possibility of using this to provide a sorting UI
        if (m_bAllowSorting)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiTableFlags table_flags_for_sort_specs = ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders;
            if (ImGui::BeginTable("for_sort_specs_only", 2, table_flags_for_sort_specs, ImVec2(0.0f, ImGui::GetFrameHeight())))
            {
                ImGui::TableSetupColumn("Index");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();
                if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                    if (sort_specs->SpecsDirty || m_bRequestSort)
                    {
                        AssetField::SortWithSortSpecs(sort_specs, Items.data(), static_cast<int>(Items.size()));
                        sort_specs->SpecsDirty = m_bRequestSort = false;
                    }
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowContentSize(ImVec2(0.0f, m_fLayoutOuterPadding + static_cast<float>(m_iLayoutLineCount) * (m_layoutItemSize.y + m_fLayoutItemSpacing)));
        if (ImGui::BeginChild("Assets", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            const float avail_width = ImGui::GetContentRegionAvail().x;
            UpdateLayoutSizes(avail_width);

            // Calculate and store start position.
            ImVec2 start_pos = ImGui::GetCursorScreenPos();
            start_pos = ImVec2(start_pos.x + m_fLayoutOuterPadding, start_pos.y + m_fLayoutOuterPadding);
            ImGui::SetCursorScreenPos(start_pos);

            // Multi-select
            ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid;

            // - Enable box-select (in 2D mode, so that changing box-select rectangle X1/X2 boundaries will affect clipped items)
            if (m_bAllowBoxSelect)
                ms_flags |= ImGuiMultiSelectFlags_BoxSelect2d;

            // - This feature allows dragging an unselected item without selecting it (rarely used)
            if (m_bAllowDragUnselected)
                ms_flags |= ImGuiMultiSelectFlags_SelectOnClickRelease;


            // When we finish implementing a more general API for this, we will obsolete this flag in favor of the new system)
            ms_flags |= ImGuiMultiSelectFlags_NavWrapX;

            ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, m_selection.Size, static_cast<int>(Items.size()));

            // Use custom selection adapter: store ID in selection (recommended)
            m_selection.UserData = this;
            //Selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { LevelBuilder* self = (LevelBuilder*)self_->UserData; return self->Items[idx].ID; };
            m_selection.ApplyRequests(ms_io);

            constexpr int item_curr_idx_to_focus = -1;
            m_bRequestDelete = false;

            // Push LayoutSelectableSpacing (which is LayoutItemSpacing minus hit-spacing, if we decide to have hit gaps between items)
            // Altering style ItemSpacing may seem unnecessary as we position every items using SetCursorScreenPos()...
            // But it is necessary for two reasons:
            // - Selectables uses it by default to visually fill the space between two items.
            // - The vertical spacing would be measured by Clipper to calculate line height if we didn't provide it explicitly (here we do).
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(m_fLayoutSelectableSpacing, m_fLayoutSelectableSpacing));

            // Rendering parameters
            constexpr ImU32 icon_type_overlay_colors[3] = { 0, IM_COL32(200, 70, 70, 255), IM_COL32(70, 170, 70, 255) };
            const ImU32 icon_bg_color = ImGui::GetColorU32(IM_COL32(35, 35, 35, 220));
            constexpr ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
            const bool display_label = (m_layoutItemSize.x >= ImGui::CalcTextSize("999").x);

            const int column_count = m_iLayoutColumnCount;
            ImGuiListClipper clipper;
            clipper.Begin(m_iLayoutLineCount, m_layoutItemStep.y);
            if (ms_io->RangeSrcItem != -1)
                clipper.IncludeItemByIndex(static_cast<int>(ms_io->RangeSrcItem) / column_count); // Ensure RangeSrc item line is not clipped.
            while (clipper.Step())
            {
                for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++)
                {
                    const int item_min_idx_for_current_line = line_idx * column_count;
                    const int item_max_idx_for_current_line = IM_MIN((line_idx + 1) * column_count, Items.size());
                    for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx)
                    {
                        AssetField* item_data = &Items[item_idx];
                        ImGui::PushID(static_cast<int>(item_data->ID));

                        // Position item
                        ImVec2 pos = ImVec2(start_pos.x + static_cast<float>(item_idx % column_count) * m_layoutItemStep.x, start_pos.y + static_cast<float>(line_idx) * m_layoutItemStep.y);
                        ImGui::SetCursorScreenPos(pos);

                        ImGui::SetNextItemSelectionUserData(item_idx);
                        bool item_is_selected = m_selection.Contains((ImGuiID)item_data->ID);
                        bool item_is_visible = ImGui::IsRectVisible(m_layoutItemSize);
                        ImGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, m_layoutItemSize);

                        // Update our selection state immediately (without waiting for EndMultiSelect() requests)
                        // because we use this to alter the color of our text/icon.
                        if (ImGui::IsItemToggledSelection()) {
                            item_is_selected = !item_is_selected;
                        }

                        // Focus (for after deletion)
                        if (item_curr_idx_to_focus == item_idx)
                            ImGui::SetKeyboardFocusHere(-1);

                        // Drag and drop
                        if (ImGui::BeginDragDropSource())
                        {

                            // Create payload with full selection OR single unselected item.
                            // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
                            if (ImGui::GetDragDropPayload() == nullptr)
                            {
                                std::vector<ImGuiID> payload_items;
                                void* it = nullptr;
                                ImGuiID id = 0;
                                if (!item_is_selected)
                                    payload_items.push_back(item_data->ID);
                                else
                                    while (m_selection.GetNextSelectedItem(&it, &id))
                                        payload_items.push_back(id);
                                ImGui::SetDragDropPayload("ASSETS_BROWSER_ITEMS", payload_items.data(), payload_items.size());
                            }


                            // Display payload content in tooltip, by extracting it from the payload data
                            // (we could read from selection, but it is more correct and reusable to read from payload)
                            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
                            const int payload_count = static_cast<int>(payload->DataSize) / static_cast<int>(sizeof(ImGuiID));
                            ImGui::Text("%d assets", payload_count);

                            ImGui::EndDragDropSource();



                        }

                        // Render icon (a real app would likely display an image/thumbnail here)
                        // Because we use ImGuiMultiSelectFlags_BoxSelect2d, clipping vertical may occasionally be larger, so we coarse-clip our rendering as well.

                        if (item_is_visible)
                        {
                            ImVec2 box_min(pos.x - 1, pos.y - 1);
                            ImVec2 box_max(box_min.x + m_layoutItemSize.x + 2, box_min.y + m_layoutItemSize.y + 2); // Dubious
                            draw_list->AddRectFilled(box_min, box_max, icon_bg_color); // Background color
                            if (m_bShowTypeOverlay && item_data->assetType != 0)
                            {
                                ImU32 type_col = icon_type_overlay_colors[item_data->assetType % IM_COUNTOF(icon_type_overlay_colors)];
                                draw_list->AddRectFilled(ImVec2(box_max.x - 2 - icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y + 2 + icon_type_overlay_size.y), type_col);
                            }
                            if (display_label)
                            {
                                ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
                                char label[64];
                                sprintf(label, "%s", item_data->assetName.c_str());
                                draw_list->AddText(ImVec2(box_min.x, box_max.y - ImGui::GetFontSize()), label_col, label);
                            }
                        }

                        ImGui::PopID();
                    }
                }
            }
            clipper.End();
            ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

            // Context menu
            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::Text("Selection: %d items", m_selection.Size);
                ImGui::Separator();
                if (ImGui::MenuItem("Delete", "Del", false, m_selection.Size > 0))
                    m_bRequestDelete = true;
                ImGui::EndPopup();
            }

            ms_io = ImGui::EndMultiSelect();
            m_selection.ApplyRequests(ms_io);

            // Zooming with Ctrl+Wheel
            if (ImGui::IsWindowAppearing())
                m_fZoomWheelAccum = 0.0f;
            if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsAnyItemActive() == false)
            {
                m_fZoomWheelAccum += io.MouseWheel;
                if (fabsf(m_fZoomWheelAccum) >= 1.0f)
                {
                    // Calculate hovered item index from mouse location
                    const float hovered_item_nx = (io.MousePos.x - start_pos.x + m_fLayoutItemSpacing * 0.5f) / m_layoutItemStep.x;
                    const float hovered_item_ny = (io.MousePos.y - start_pos.y + m_fLayoutItemSpacing * 0.5f) / m_layoutItemStep.y;
                    const int hovered_item_idx = (static_cast<int>(hovered_item_ny) * m_iLayoutColumnCount) + static_cast<int>(hovered_item_nx);
                    //ImGui::SetTooltip("%f,%f -> item %d", hovered_item_nx, hovered_item_ny, hovered_item_idx); // Move those 4 lines in block above for easy debugging

                    // Zoom
                    m_fIconSize *= powf(1.1f, m_fZoomWheelAccum);
                    m_fIconSize = IM_CLAMP(m_fIconSize, 16.0f, 128.0f);
                    m_fZoomWheelAccum -= static_cast<float>(static_cast<int>(m_fZoomWheelAccum));
                    UpdateLayoutSizes(avail_width);

                    // Manipulate scroll to that we will land at the same Y location of currently hovered item.
                    // - Calculate next frame position of item under mouse
                    // - Set new scroll position to be used in next ImGui::BeginChild() call.
                    float hovered_item_rel_pos_y = (static_cast<float>(hovered_item_idx) / static_cast<float>(m_iLayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * m_layoutItemStep.y;
                    hovered_item_rel_pos_y += ImGui::GetStyle().WindowPadding.y;
                    float mouse_local_y = io.MousePos.y - ImGui::GetWindowPos().y;
                    ImGui::SetScrollY(hovered_item_rel_pos_y - mouse_local_y);
                }
            }
        }
        ImGui::EndChild();

        ImGui::Text("Selected: %d/%d items", m_selection.Size, static_cast<int>(Items.size()));
        ImGui::End();
    }





}
