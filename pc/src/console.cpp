#include "console.hpp"

#include "transmit.hpp"
#include "common.hpp"
#include "editor.hpp"

#include "imgui.h"

#include <iostream>
#include <unistd.h>

namespace tas
{
    namespace console
    {
        char input[64] = {0};
        std::deque<std::string> log_items;
        void log(std::string str)
        {
            log_items.push_back(str);
        }
        void mainLoop()
        {
            ImGui::SetNextWindowSize(MASTER_WINDOW_SIZE - CONSOLE_WINDOW_POS);
            ImGui::SetNextWindowPos(CONSOLE_WINDOW_POS);
            if (ImGui::Begin("Console", &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("log", ImVec2(0, -footer_height_to_reserve)))
                {
                    while (log_items.size() > MAX_LOG_ENTRIES) log_items.pop_front();
                    for (auto cur : log_items) ImGui::Text(cur.c_str());
                } ImGui::EndChild();
                if (ImGui::InputText("input", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
                {
                    parse(input);
                    input[0] = '\0';
                }
            } ImGui::End();
        }

        void parse(std::string str)
        {
            // int prev_frame = 0;
            auto argv = tokenize(str, ' ');
            // if (argv[0][0] >= '0' && argv[0][0] <= '9')
            // {
            //     // first arg is int, nx-TAS format
            //     tas::script::frameInputMsg cur(str);
            //     transmit::sendCommand(("advance " + std::to_string(cur.frame - prev_frame)).c_str());
            //     parse(cur);
            //     transmit::sendCommand("advance"); // advance one frame

            //     // reset
            //     transmit::sendCommand("resetControllerState");
            //     prev_frame = cur.frame + 1;
            // }
            // else
            // {
                if (argv[0] == "load")
                {
                    editor::openFile(argv[1].c_str());
                }
                else if (argv[0] == "sleep")
                {
                    try
                    {
                        usleep(std::stof(argv[1]) * 1000);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
                else if (argv[0] == "run")
                {
                    /* code */
                }
                
                else
                {
                    transmit::sendCommand(str);
                }
            // }
        }
    } // namespace console
    
} // namespace tas
