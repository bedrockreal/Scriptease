#ifndef TAS_COMMON_HPP
#define TAS_COMMON_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#define MASTER_WINDOW_SIZE  sf::Vector2u(1200, 820)
#define CONTROL_WINDOW_SIZE 200
#define CONTROL_WINDOW_POS  sf::Vector2u(MASTER_WINDOW_SIZE.x - 2 * CONTROL_WINDOW_SIZE, 20)
#define CONSOLE_WINDOW_POS  sf::Vector2u(MASTER_WINDOW_SIZE.x - 2 * CONTROL_WINDOW_SIZE, 20 + 2 * CONTROL_WINDOW_SIZE)
#define EDITOR_WINDOW_POS   sf::Vector2u(0, 20)
#define MAIN_BUTTON_SIZE    40
#define DIR_BUTTON_SIZE     52
#define MAX_LOG_ENTRIES     100

namespace tas
{
    extern bool windowOpen;
    extern bool showSaveFileWindow_Flag;
    extern bool showOpenFileWindow_Flag;
    extern bool showRunFileWindow_Flag;
    std::vector<std::string> tokenize(std::string input, char delim);
    void clearFlags();
} // namespace tas

#endif