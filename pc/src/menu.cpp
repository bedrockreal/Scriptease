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
                        script::appendLines(script::editorInputSeq);
                    }
                    if (ImGui::MenuItem("Insert Line(s)", "Ctrl+I"))
                    {
                        script::insertLines(script::editorInputSeq);
                    }
                    if (ImGui::MenuItem("Delete Line(s)", "Delete"))
                    {
                        script::deleteLines(script::editorInputSeq);
                    }
                    if (ImGui::MenuItem("Duplicate Line(s)", "Ctrl+D"))
                    {
                        script::duplicateLines(script::editorInputSeq);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Options"))
                {
                    if (ImGui::MenuItem("Connect", "Alt+C"))
                    {
                        transmit::setUpConnection("192.168.86.211", 6000);
                    }
                    if (ImGui::MenuItem("Pause", "Ctrl+Shift+P"))
                    {
                        transmit::sendCommand("pause");
                    }
                    if (ImGui::MenuItem("Unpause", "Ctrl+Shift+U"))
                    {
                        transmit::sendCommand("Unpause");
                    }
                    if (ImGui::MenuItem("Advance 1F", "Ctrl+Shift+SPACE"))
                    {
                        tas::transmit::sendCommand("advance");
                    }
                    if (ImGui::MenuItem("Run Current", "F9"))
                    {
                        tas::script::run(tas::script::editorInputSeq);
                    }
                    if (ImGui::MenuItem("Run File", "F10"))
                    {
                        tas::script::runFile();
                    }
                    if (ImGui::MenuItem("Cancel Run", "Ctrl+Shift+C"))
                    {
                        script::frameToRun = script::runInputSeq.size();
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
