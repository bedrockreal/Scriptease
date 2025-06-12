#include "control.hpp"
#include "common.hpp"
#include "transmit.hpp"

#include "imgui.h"

namespace tas
{
    namespace control
    {
        std::unordered_set<std::string> keys_pressed;
        void mainLoop()
        {
            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_WINDOW_SIZE, CONTROL_WINDOW_SIZE));
            ImGui::SetNextWindowPos(CONTROL_WINDOW_POS + sf::Vector2u(0, 0));
            if (ImGui::Begin("Main Buttons", &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "A", "press A", "release A");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "B", "press B", "release B");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "X", "press X", "release X");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "Y", "press Y", "release Y");
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "L", "press L", "release L");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "R", "press R", "release R");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "ZL", "press ZL", "release ZL");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "ZR", "press ZR", "release ZR");
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "+", "press PLUS", "release PLUS");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "-", "press MINUS", "release MINUS");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "HOME", "press HOME", "release HOME");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "CAP", "press CAPTURE", "release CAPTURE");
            } ImGui::End();

            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_WINDOW_SIZE, CONTROL_WINDOW_SIZE));
            ImGui::SetNextWindowPos(CONTROL_WINDOW_POS + sf::Vector2u(CONTROL_WINDOW_SIZE, 0));
            if (ImGui::Begin("D-Pad", &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                ImGui::Columns(3, "dpadcol", false);
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "^##D", "press DUP", "release DUP");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "<##D", "press DLEFT", "release DLEFT");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, ">##D", "press DRIGHT", "release DRIGHT");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "v##D", "press DDOWN", "release DDOWN");
            } ImGui::End();

            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_WINDOW_SIZE, CONTROL_WINDOW_SIZE));
            ImGui::SetNextWindowPos(CONTROL_WINDOW_POS + sf::Vector2u(0, CONTROL_WINDOW_SIZE));
            if (ImGui::Begin("Left Joystick", &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                ImGui::Columns(3, "lstickcol", false);
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "^##L", "setStick LEFT 0 0x7FFF", "setStick LEFT 0 0");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "<##L", "setStick LEFT -0x7FFF 0", "setStick LEFT 0 0");

                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "O##L", "press LSTICK", "release LSTICK");

                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, ">##L", "setStick LEFT 0x7FFF 0", "setStick LEFT 0 0");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "v##L", "setStick LEFT 0 -0x7FFF", "setStick LEFT 0 0");
            } ImGui::End();

            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_WINDOW_SIZE, CONTROL_WINDOW_SIZE));
            ImGui::SetNextWindowPos(CONTROL_WINDOW_POS + sf::Vector2u(CONTROL_WINDOW_SIZE, CONTROL_WINDOW_SIZE));
            if (ImGui::Begin("Right Joystick", &windowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                ImGui::Columns(3, "rstickcol", false);
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "^##R", "setStick RIGHT 0 0x7FFF", "setStick RIGHT 0 0");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "<##R", "setStick RIGHT -0x7FFF 0", "setStick RIGHT 0 0");

                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "O##R", "press RSTICK", "release RSTICK");

                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, ">##R", "setStick RIGHT 0x7FFF 0", "setStick RIGHT 0 0");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "v##R", "setStick RIGHT 0 -0x7FFF", "setStick RIGHT 0 0");
            } ImGui::End();
        }

        void imgui_add_switch_key(const int size, const std::string lbl, const std::string press_cmd, const std::string release_cmd)
        {
            if (ImGui::Button(lbl.c_str(), ImVec2(size, size)))
            {
                if (keys_pressed.find(lbl) == keys_pressed.end())
                {
                    tas::transmit::sendCommand(press_cmd);
                    keys_pressed.emplace(lbl);
                }
            }
            else
            {
                if (keys_pressed.find(lbl) != keys_pressed.end())
                {
                    tas::transmit::sendCommand(release_cmd);
                    keys_pressed.erase(lbl);
                }
            }
        }
    } // namespace control
    
} // namespace tas
