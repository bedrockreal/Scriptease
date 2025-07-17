#include "control.hpp"
#include "common.hpp"
#include "transmit.hpp"
#include "joystick.hpp"

#include "imgui.h"

#include <format>
#include <iostream>

namespace tas
{
    namespace control
    {
        std::unordered_set<std::string> keys_pressed;
        sf::Vector2f joy_pos[2] = {{0, 0}, {0, 0}};
        bool joy_clicked[2] = {0, 0};

        void mainLoop()
        {
            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_PANEL_SIZE, CONTROL_PANEL_SIZE));
            ImGui::SetNextWindowPos(CONTROL_PANEL_POS + sf::Vector2u(0, 0));
            if (ImGui::Begin("Main Buttons", &window_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "A", "press(KEY_A)", "release(KEY_A)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "B", "press(KEY_B)", "release(KEY_B)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "X", "press(KEY_X)", "release(KEY_X)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "Y", "press(KEY_Y)", "release(KEY_Y)");
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "L", "press(KEY_L)", "release(KEY_L)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "R", "press(KEY_R)", "release(KEY_R)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "ZL", "press(KEY_ZL)", "release(KEY_ZL)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "ZR", "press(KEY_ZR)", "release(KEY_ZR)");
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "+", "press(KEY_PLUS)", "release(KEY_PLUS)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "-", "press(KEY_MINUS)", "release(KEY_MINUS)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "HOME", "press(KEY_HOME)", "release(KEY_HOME)");
                ImGui::SameLine();
                imgui_add_switch_key(MAIN_BUTTON_SIZE, "CAP", "press(KEY_CAPTURE)", "release(KEY_CAPTURE)");
            } ImGui::End();

            ImGui::SetNextWindowSize(sf::Vector2u(CONTROL_PANEL_SIZE, CONTROL_PANEL_SIZE));
            ImGui::SetNextWindowPos(CONTROL_PANEL_POS + sf::Vector2u(CONTROL_PANEL_SIZE, 0));
            if (ImGui::Begin("D-Pad", &window_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                ImGui::Columns(3, "dpadcol", false);
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "^##D", "press(KEY_DUP)", "release(KEY_DUP)");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "<##D", "press(KEY_DLEFT)", "release(KEY_DLEFT)");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, ">##D", "press(KEY_DRIGHT)", "release(KEY_DRIGHT)");

                ImGui::NextColumn();
                ImGui::NextColumn();
                imgui_add_switch_key(DIR_BUTTON_SIZE, "v##D", "press(KEY_DDOWN)", "release(KEY_DDOWN)");
            } ImGui::End();

            ImGui::SetNextWindowSize(sf::Vector2u(2 * CONTROL_PANEL_SIZE + 40, CONTROL_PANEL_SIZE + 40));
            ImGui::SetNextWindowPos(CONTROL_PANEL_POS + sf::Vector2u(0, CONTROL_PANEL_SIZE));
            if (ImGui::Begin("Joysticks"))
            {
                bool update_joy = 0;
                for (int i = 0; i < 2; ++i)
                {
                    bool prev_clicked = joy_clicked[i];
                    if (ImGui::JoystickSlider(joy_pos[i], joy_clicked[i]))
                    {
                        update_joy = 1;
                    }
                    else if (prev_clicked)
                    {
                        joy_pos[i] = {0, 0};
                        update_joy = 1;
                    }
                    if (i == 0) ImGui::SameLine();
                }

                if (update_joy)
                {
                    sf::Vector2i int_joy_l(joy_pos[0].x * MAX_JOY_COORD_ABS, -joy_pos[0].y * MAX_JOY_COORD_ABS);
                    sf::Vector2i int_joy_r(joy_pos[1].x * MAX_JOY_COORD_ABS, -joy_pos[1].y * MAX_JOY_COORD_ABS);

                    std::string cmd = std::format("setControllerState(0, {}, {}, {}, {})", int_joy_l.x, int_joy_l.y, int_joy_r.x, int_joy_r.y);
                    transmit::sendCommand(cmd);
                }
            } ImGui::End();
        }

        void imgui_add_switch_key(const int size, const std::string lbl, const std::string press_cmd, const std::string release_cmd)
        {
            ImGui::Button(lbl.c_str(), ImVec2(size, size));
            if (ImGui::IsItemActive())
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
