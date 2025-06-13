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
                if (ImGui::Begin("Open File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showOpenFileWindow_Flag = false;
                        editor::openFile(buf);
                    }
                    ImGui::End();
                }
            }
            if (showSaveFileWindow_Flag)
            {
                if (ImGui::Begin("Save File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showSaveFileWindow_Flag = false;
                        if (buf[0] == 0) editor::saveFile(editor::loadedFileName);
                        else editor::saveFile(buf);
                    }
                    ImGui::End();
                }
            }
            if (showRunFileWindow_Flag)
            {
                if (ImGui::Begin("Run File"))
                {
                    char buf[64] = {0};
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText("Enter File Name", buf, 64, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        showRunFileWindow_Flag = false;
                        script::runFile(buf);
                    }
                    ImGui::End();
                }
            }
        }
    } // namespace popup
} // namespace tas
