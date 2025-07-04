#include "editor.hpp"
#include "script.hpp"
#include "common.hpp"

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
            if (showRunFileWindow_Flag)
            {
                ImGui::OpenPopup("Run File");
                if (ImGui::BeginPopupModal("Run File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showRunFileWindow_Flag = false;
                        script::runFile(buf);
                    }
                    ImGui::EndPopup();
                }
            }
        }
    } // namespace popup
} // namespace tas
