#include "console.hpp"

#include "transmit.hpp"
#include "common.hpp"
#include "editor.hpp"
#include "script.hpp"

#include "imgui.h"

#include <iostream>
#include <unistd.h>

namespace tas
{
    namespace console
    {
        bool focusHere = false;
        char input[64] = {0};
        std::deque<std::string> log_items;
        void log(std::string str)
        {
            log_items.push_back(str);
        }
        void mainLoop()
        {
            if (ImGui::BeginTabItem("Console"))
            {
                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("log", ImVec2(0, -footer_height_to_reserve)))
                {
                    while (log_items.size() > MAX_LOG_ENTRIES) log_items.pop_front();
                    for (auto cur : log_items) ImGui::Text(cur.c_str());
                } ImGui::EndChild();
                if (focusHere) ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("input", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
                {
                    focusHere = true;
                    parse(input);
                    input[0] = '\0';
                }
                else focusHere = false;

                ImGui::EndTabItem();
            }
        }

        void parse(std::string str)
        {
            try
            {
                auto argv = tokenize(str, ' ');
                if (argv[0] == "load")
                {
                    editor::openFile(argv[1].c_str());
                }
                else if (argv[0] == "clear")
                {
                    log_items.clear();
                }
                else
                {
                    transmit::sendCommand(str);
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    } // namespace console
    
} // namespace tas
