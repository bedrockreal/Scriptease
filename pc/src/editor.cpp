#include "editor.hpp"
#include "script.hpp"
#include "common.hpp"
#include "menu.hpp"

#include "imgui.h"

#include <fstream>
#include <iostream>

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
        std::string editorFileName = "";

        void mainLoop()
        {
            ImGui::SetNextWindowSize(sf::Vector2u(CONSOLE_WINDOW_POS.x - EDITOR_WINDOW_POS.x, MASTER_WINDOW_SIZE.y - EDITOR_WINDOW_POS.y));
            ImGui::SetNextWindowPos(EDITOR_WINDOW_POS);
            char title[64];
            sprintf(title, "TAS Editor: %s", editorFileName.empty() ? "Untitled" : editorFileName.c_str());
            if (ImGui::Begin(title, &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                if (ImGui::BeginTable("table", tas::editor::NUM_OF_COLS, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY))
                {
                    for (int j = 0; j < tas::editor::NUM_OF_COLS; ++j)
                    {
                        ImGui::TableSetupColumn(tas::editor::head[j], ImGuiTableColumnFlags_None, strlen(tas::editor::head[j]) * (j >= 1 && j <= 2 ? 20 : 10));
                    }
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    for (int i = 0; i < tas::script::editorInputSeq.size(); ++i)
                    {
                        char label[32];
                        sprintf(label, "%d", i);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        if (ImGui::Selectable(label, script::editorInputSeq[i].isSelected))
                        {
                            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl)))
                            {
                                for (int j = 0; j < tas::script::editorInputSeq.size(); ++j) script::editorInputSeq[j].isSelected = 0;
                            }
                            script::editorInputSeq[i].isSelected ^= 1;
                        }
                        
                        sprintf(label, "##%dL", i); ImGui::TableNextColumn(); ImGui::InputInt2(label, tas::script::editorInputSeq[i].joyL);
                        sprintf(label, "##%dR", i); ImGui::TableNextColumn(); ImGui::InputInt2(label, tas::script::editorInputSeq[i].joyR);
                        for (int j = 0; j < tas::editor::NUM_OF_COLS - 3; ++j)
                        {
                            sprintf(label, "##%d;%d", i, j);
                            ImGui::TableNextColumn(); ImGui::Checkbox(label, &tas::script::editorInputSeq[i].isPressed[j]);
                        }
                    }
                    ImGui::EndTable();
                }
            } ImGui::End();
        }

        void saveFile(std::string filename)
        {
            if (tas::script::editorInputSeq.empty())
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
                for (int i = 0; i < tas::script::editorInputSeq.size(); ++i) if (!tas::script::editorInputSeq[i].isIdle())
                {
                    std::string line = std::to_string(i) + " " + tas::script::editorInputSeq[i].getNxTasStr() + "\n";
                    file.write(line.c_str(), line.length());
                }
                editorFileName = filename;
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
                script::loadFromFile(filename, script::editorInputSeq);
                editorFileName = filename;
            }
        }
    } // namespace editor
} // namespace tas