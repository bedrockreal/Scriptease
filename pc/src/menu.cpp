#include "menu.hpp"
#include "transmit.hpp"
#include "editor.hpp"
#include "script.hpp"

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
                    if (ImGui::MenuItem("Open", "Ctrl+O"))
                    {
                        tas::editor::openFile();
                    }
                    if (ImGui::MenuItem("Save", "Ctrl+S"))
                    {
                        tas::editor::saveFile();
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
                    // if (ImGui::MenuItem("Connect", "Alt+C"))
                    // {
                    //     transmit::setUpConnection("192.168.86.211", 6000);
                    // }
                    if (ImGui::BeginMenu("Connect"))
                    {
                        char addr[32] = "192.168.86.228\0";
                        int port = 6000;
                        ImGui::InputText("Address", addr, 32, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::InputInt("Port", &port, 1, 100);
                        if (ImGui::Button("Confirm"))
                        {
                            transmit::setUpConnection(addr, port);
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::MenuItem("Pause", "Ctrl+Shift+P"))
                    {
                        transmit::sendCommand("attach()");
                    }
                    if (ImGui::MenuItem("Unpause", "Ctrl+Shift+U"))
                    {
                        transmit::sendCommand("Unpause");
                    }
                    if (ImGui::MenuItem("Advance 1F", "Ctrl+Shift+SPACE"))
                    {
                        tas::transmit::sendCommand("advanceFrames(1)");
                    }
                    if (ImGui::MenuItem("Run Current", "F9"))
                    {
                        tas::script::run(tas::editor::loaded_input_seq);
                    }
                    if (ImGui::MenuItem("Run File", "F10"))
                    {
                        tas::script::runFile();
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
