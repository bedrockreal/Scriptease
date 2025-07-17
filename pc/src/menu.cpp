#include "menu.hpp"
#include "transmit.hpp"
#include "editor.hpp"
#include "script.hpp"
#include "common.hpp"

namespace tas
{
    namespace menu
    {
        void mainLoop()
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                    {
                        // pass
                    }
                    if (ImGui::MenuItem("Open", "Ctrl+O"))
                    {
                        tas::editor::openFileNoArgs();
                    }
                    if (ImGui::MenuItem("Save", "Ctrl+S"))
                    {
                        tas::editor::saveFileNoArgs();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo (not implemented)", "Ctrl+Z"))
                    {

                    }
                    if (ImGui::MenuItem("Redo (not implemented)", "Ctrl+Y"))
                    {
                        
                    }
                    if (ImGui::MenuItem("Append Line", "Ctrl+T"))
                    {
                        editor::loaded_input_seq.appendLines();
                    }
                    if (ImGui::MenuItem("Insert Line(s)", "Ctrl+I"))
                    {
                        editor::loaded_input_seq.insertSelected();
                    }
                    if (ImGui::MenuItem("Delete Line(s)", "Delete"))
                    {
                        editor::loaded_input_seq.deleteSelected();
                    }
                    if (ImGui::MenuItem("Duplicate Line(s)", "Ctrl+D"))
                    {
                        editor::loaded_input_seq.duplicateSelected();
                    }
                    if (ImGui::MenuItem("Swap Line(s) Up", "UP"))
                    {
                        editor::loaded_input_seq.swapSelectedUp();
                    }
                    if (ImGui::MenuItem("Swap Line(s) Down", "DOWN"))
                    {
                        editor::loaded_input_seq.swapSelectedDown();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Options"))
                {
                    if (ImGui::MenuItem("Connect", "Alt+C"))
                    {
                        showInputIPWindow_Flag = true;
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Pause", "Ctrl+Shift+P"))
                    {
                        transmit::sendCommand("attach()");
                    }
                    if (ImGui::MenuItem("Unpause", "Ctrl+Shift+U"))
                    {
                        transmit::sendCommand("detach()");
                    }
                    if (ImGui::MenuItem("Advance 1F", "Ctrl+Shift+SPACE"))
                    {
                        tas::transmit::sendCommand("advTAS(1)");
                    }
                    if (ImGui::MenuItem("Cancel Run", "Ctrl+Shift+C"))
                    {
                        script::runCancel();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }

        // void showPopUp(const char* title, const char* prompt, void (*f)(const char*))
        // {
        //     
        // }
    } // namespace menu
    
} // namespace tas
