#ifndef TAS_CONTROL_HPP
#define TAS_CONTROL_HPP

#include <string>
#include <unordered_set>

#include <SFML/Graphics.hpp>

namespace tas
{
    namespace control
    {
        extern std::unordered_set<std::string> keys_pressed;
        void mainLoop();
        void imgui_add_switch_key(const int size, const std::string lbl, const std::string press_cmd, const std::string release_cmd);

        extern sf::Vector2f joy_pos[2];
        extern bool joy_clicked[2];
    } // namespace control
    
} // namespace tas

#endif