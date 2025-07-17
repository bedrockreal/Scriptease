#include "editor.hpp"
#include "script.hpp"
#include "common.hpp"
#include "transmit.hpp"

#include "imgui.h"

namespace tas
{
    namespace popup
    {
        void mainLoop()
        {
            if (showOpenFileWindow_Flag)
            {
                ImGui::OpenPopup("Open File");
                if (ImGui::BeginPopupModal("Open File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showOpenFileWindow_Flag = false;
                        editor::openFile(buf);
                    }
                    ImGui::EndPopup();
                }
            }
            if (showSaveFileWindow_Flag)
            {
                ImGui::OpenPopup("Save File");
                if (ImGui::BeginPopupModal("Save File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showSaveFileWindow_Flag = false;
                        if (buf[0] == 0) editor::saveFile(editor::editor_file_name);
                        else editor::saveFile(buf);
                    }
                    ImGui::EndPopup();
                }
            }

            if (showInputIPWindow_Flag)
            {
                char ip[32] = "192.168.86.234\0";
                int port = 6000;
                ImGui::OpenPopup("Connect to Switch");
                if (ImGui::BeginPopupModal("Connect to Switch"))
                {
                    ImGui::InputText("Address", ip, 32, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::InputInt("Port", &port, 1, 100);
                    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {
                        if (transmit::setUpConnection(ip, port)) showInputIPWindow_Flag = false;
                    }
                    ImGui::EndPopup();
                }
            }
        }
    } // namespace popup
} // namespace tas
