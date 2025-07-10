#ifndef TAS_JOYSTICK_HPP
#define TAS_JOYSTICK_HPP

#include "imgui.h"
#include "imgui_internal.h"

#include <sys/types.h>
#include <cmath>

// see https://github.com/ocornut/imgui/issues/5186
namespace ImGui
{
    bool JoystickSlider(auto& _vector2f, bool& button_clicked, float scale = 100.f, float b_scale = 15.f, uint bg_color = IM_COL32(0, 0, 0, 155), uint button_color = IM_COL32(215, 215, 215, 255), const uint& mouse_button = 0u)
    {
        ImDrawList* draw_list = GetWindowDrawList();
        const auto& p = GetCursorScreenPos();
        const auto& mouse = GetIO().MousePos;
        draw_list->AddCircleFilled(ImVec2(p.x + scale, p.y + scale), scale, bg_color, 50);
        auto button_x = _vector2f.x * (scale - b_scale) + p.x + scale;
        auto button_y = _vector2f.y * (scale - b_scale) + p.y + scale;
        static float toward = 0.f;
        
        // ButtonBehavior(ImRect({ button_x - b_scale, button_y - b_scale}, { button_x + b_scale, button_y + b_scale }), GetCurrentWindow()->ID, nullptr, nullptr);
        float mouse_dist_to_button = sqrtf(pow(mouse.x - button_x, 2) + pow(mouse.y - button_y, 2));
        if (GetIO().MouseDown[mouse_button])
        {
            if (mouse_dist_to_button < b_scale)
                button_clicked = true;
        }
        else button_clicked = false;

        if (button_clicked) {
            button_x = mouse.x;
            button_y = mouse.y;
        };
        toward = -atan2(button_x - p.x - scale, button_y - p.y - scale);
        if (sqrtf(pow(p.x - button_x + scale, 2) + pow(p.y - button_y + scale, 2)) > scale - b_scale) {
            button_x = p.x + scale + cos(toward - 0.5f * 3.14f) * -(scale-b_scale);
            button_y = p.y + scale + sin(toward - 0.5f * 3.14f) * -(scale-b_scale);
        };
        draw_list->AddCircleFilled(ImVec2(button_x, button_y), b_scale, button_color, 25);
        Dummy(ImVec2(scale*2, scale*2));
        _vector2f.x = (button_x - p.x - scale) / (scale - b_scale);
        _vector2f.y = (button_y - p.y - scale) / (scale - b_scale);

        // std::cout << button_x << ' ' << button_y << std::endl;

        return button_clicked;
    };
} // namespace ImGui

#endif