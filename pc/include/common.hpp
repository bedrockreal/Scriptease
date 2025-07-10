#ifndef TAS_COMMON_HPP
#define TAS_COMMON_HPP

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include <vector>
#include <string>

#define LINE_HEIGHT                 ImGui::GetTextLineHeightWithSpacing()

#define MASTER_WINDOW_SIZE          sf::Vector2u(1310, 820)
#define CONTROL_PANEL_SIZE          200
#define EDITOR_PANEL_SIZE           sf::Vector2u(CONSOLE_PANEL_POS.x - EDITOR_PANEL_POS.x, MASTER_WINDOW_SIZE.y - EDITOR_PANEL_POS.y)
#define EDITOR_TABLE_SIZE           sf::Vector2u(700, EDITOR_PANEL_SIZE.y - LINE_HEIGHT)
#define EDITOR_JOYSTICK_CHILD_SIZE  sf::Vector2u(CONTROL_PANEL_SIZE, EDITOR_TABLE_SIZE.y)
#define CONTROL_PANEL_POS           sf::Vector2u(MASTER_WINDOW_SIZE.x - 2 * CONTROL_PANEL_SIZE, LINE_HEIGHT)
#define CONSOLE_PANEL_POS           sf::Vector2u(MASTER_WINDOW_SIZE.x - 2 * CONTROL_PANEL_SIZE, LINE_HEIGHT + 2 * CONTROL_PANEL_SIZE)
#define EDITOR_PANEL_POS            sf::Vector2u(0, LINE_HEIGHT)
// #define JOYSTICK_PANEL_POS  sf::vector2u(MASTER_WINDOW_SIZE.x - 3 * CONTROL_PANEL_SIZE, 20);
#define MAIN_BUTTON_SIZE    40
#define DIR_BUTTON_SIZE     52
#define MAX_LOG_ENTRIES     100

#define MAX_JOY_COORD_ABS   0x7FFF

// extern sf::RenderWindow window;
static bool window_open = true;
extern std::vector<sf::Drawable*> pending_to_draw;

namespace tas
{
    extern bool showSaveFileWindow_Flag;
    extern bool showOpenFileWindow_Flag;
    extern bool showRunFileWindow_Flag;
    std::vector<std::string> tokenize(std::string input, char delim);
    void clearFlags();
} // namespace tas

#endif