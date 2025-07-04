#ifndef TAS_CONTROL_HPP
#define TAS_CONTROL_HPP

#include <string>
#include <unordered_set>

namespace tas
{
    namespace control
    {
        extern std::unordered_set<std::string> keys_pressed;
        void mainLoop();
        void imgui_add_switch_key(const int size, const std::string lbl, const std::string press_cmd, const std::string release_cmd);
    } // namespace control
    
} // namespace tas

#endif