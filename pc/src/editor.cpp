#include "editor.hpp"
#include "common.hpp"
#include "menu.hpp"

#include "imgui.h"

#include <fstream>
#include <iostream>
#include <numeric>

namespace tas
{
    namespace editor
    {
        const int NUM_OF_COLS = 19;
        const char* const head[] = {
            "Frame\0",
            "L-Stick\0",
            "R-Stick\0",
            "A\0",
            "B\0",
            "X\0", 
            "Y\0",
            "LSTICK\0", 
            "RSTICK\0",
            "L\0",
            "R\0",
            "ZL\0",
            "ZR\0",
            "+\0",
            "-\0",
            "<\0",
            "^\0",
            ">\0",
            "v\0"
        };
        std::string editor_file_name = "";
        inputSeqWithSelection loaded_input_seq;
        int inputSeqWithSelection::_id = 0;
        joystick joy_l({0, 0}, 80);
        joystick joy_r({0, 0}, 80);

        inline bool inputSeqWithSelection::isSelected(int i)
        {
            return selection.Contains(items_id[i]);
        }

        std::vector<int> inputSeqWithSelection::getSelectedIdx()
        {
            std::vector<int> ret;
            for (int i = 0; i < size(); ++i)
                if (selection.Contains(items_id[i])) ret.push_back(i);
            std::cout << ret.size() << std::endl;
            for (int i : ret) std::cout << i << ' '; std::cout << std::endl;
            return ret;
        }

        void inputSeqWithSelection::loadFromFile(std::string filename)
        {
            inputSeq::loadFromFile(filename);
            syncID();
        }

        void inputSeqWithSelection::appendLines(int cnt)
        {
            while (cnt-- > 0)
            {
                items_id.push_back(_id++);
                push_back(script::frameInputMsg());
            }
        }

        void inputSeqWithSelection::insertSelected()
        {
            auto idx = getSelectedIdx();
            insertID(idx);
            insertLines(idx);
        }
        void inputSeqWithSelection::duplicateSelected()
        {
            auto idx = getSelectedIdx();
            duplicateID(idx);
            duplicateLines(idx);
        }
        void inputSeqWithSelection::deleteSelected()
        {
            auto idx = getSelectedIdx();
            selection.Clear();
            deleteID(idx);
            deleteLines(idx);
        }

        void inputSeqWithSelection::syncID()
        {
            while (items_id.size() > size()) items_id.pop_back();
            while (items_id.size() < size()) items_id.push_back(_id++);
        }

        void inputSeqWithSelection::insertID(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0; --j)
            {
                int i = idx[j];
                items_id.insert(items_id.begin() + i, _id++);
            }
        }

        void inputSeqWithSelection::deleteID(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0; --j)
            {
                int i = idx[j];
                items_id.erase(items_id.begin() + i);
            }
        }

        void inputSeqWithSelection::duplicateID(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0;)
            {
                int r = idx[j];
                int l = r;
                while (1)
                {
                    --j;
                    if (j >= 0 && idx[j] == l - 1) --l;
                    else break;
                };
                for (int i = r; i >= l; --i)
                {
                    items_id.insert(items_id.begin() + l, _id++);
                }
            }
        }

        void mainLoop()
        {
            ImGui::SetNextWindowSize(EDITOR_PANEL_SIZE);
            ImGui::SetNextWindowPos(EDITOR_PANEL_POS);
            char title[64];
            sprintf(title, "TAS Editor: %s", editor_file_name.empty() ? "Untitled" : editor_file_name.c_str());
            if (ImGui::Begin(title, &window_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
            {   
                const static ImGuiTableFlags table_flags = ImGuiTableFlags_BordersInner | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_ScrollY;
                if (ImGui::BeginTable("table", NUM_OF_COLS, table_flags))
                {
                    for (int j = 0; j < editor::NUM_OF_COLS; ++j)
                    {
                        ImGui::TableSetupColumn(head[j]);
                    }
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ImGuiMultiSelectFlags_BoxSelect1d, loaded_input_seq.selection.Size, loaded_input_seq.size());

                    for (int i = 0; i < editor::loaded_input_seq.size(); ++i)
                    {
                        char label[32];
                        sprintf(label, "%d", i);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::SetNextItemSelectionUserData(loaded_input_seq.items_id[i]);
                        ImGui::Selectable(label, loaded_input_seq.isSelected(i), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap);
                        
                        sprintf(label, "##%dL", i); ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
                        bool edited_stick = 0;
                        if (ImGui::InputInt2(label, editor::loaded_input_seq[i].joyL)) edited_stick = 1;
                        sprintf(label, "##%dR", i); ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
                        if (ImGui::InputInt2(label, editor::loaded_input_seq[i].joyR)) edited_stick = 1;
                        if (edited_stick) loaded_input_seq[i].clampJoystickCoords();

                        for (int j = 0; j < NUM_OF_COLS - 3; ++j)
                        {
                            sprintf(label, "##%d;%d", i, j);
                            ImGui::TableNextColumn(); ImGui::Checkbox(label, &editor::loaded_input_seq[i].isPressed[j]);
                        }
                    }
                    ms_io = ImGui::EndMultiSelect();
                    loaded_input_seq.selection.ApplyRequests(ms_io);

                    ImGui::EndTable();
                }
                // joystick panel
                ImGui::SameLine();
                joy_l.setPosition({800, 400});
                pending_to_draw.push_back(&joy_l);
            } ImGui::End();
        }

        void saveFile(std::string filename)
        {
            if (tas::editor::loaded_input_seq.empty())
            {
                // Show msg
                return;
            }
            if (filename.empty())
            {
                // Show pop-up
                showSaveFileWindow_Flag = true;
            }
            else
            {
                std::ofstream file((std::string("scripts/") + std::string(filename).c_str()));
                for (int i = 0; i < tas::editor::loaded_input_seq.size(); ++i) if (!tas::editor::loaded_input_seq[i].isIdle())
                {
                    std::string line = std::to_string(i) + " " + tas::editor::loaded_input_seq[i].getNxTasStr() + "\n";
                    file.write(line.c_str(), line.length());
                }
                editor_file_name = filename;
            }
        }

        void openFile(std::string filename)
        {
            if (filename.empty())
            {
                // Show pop-up
                showOpenFileWindow_Flag = true;
            }
            else
            {
                loaded_input_seq.loadFromFile(filename);
                // loaded_input_seq.syncID();
                editor_file_name = filename;
            }
        }
    } // namespace editor
} // namespace tas