#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cerrno>
#include <unordered_set>
#include <thread>
#include <functional>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "transmit.hpp"

#include "editor.hpp"
#include "menu.hpp"
#include "control.hpp"
#include "console.hpp"
#include "common.hpp"
#include "script.hpp"
#include "popup.hpp"

sf::RenderWindow window(sf::VideoMode(MASTER_WINDOW_SIZE), "Scriptease 0.1.0");
std::vector<sf::Drawable*> pending_to_draw;

// struct shortcut
// {
//     sf::Keyboard::Scancode final_press;
//     std::vector<sf::Keyboard::Scancode> hold;
//     // std::function<void()> *callback;
//     void (*callback)();

//     shortcut(sf::Keyboard::Scancode m_final_press, std::vector<sf::Keyboard::Scancode> m_hold, void (*m_callback)())
//     {
//         final_press = m_final_press;
//         hold = m_hold;
//         callback = m_callback;
//     }
// };

// const shortcut shortcuts[] =
// {
//     shortcut(sf::Keyboard::Scancode::O, {sf::Keyboard::Scancode::LControl}, &tas::editor::openFileNoArgs),
//     shortcut(sf::Keyboard::Scancode::S, {sf::Keyboard::Scancode::LControl}, &tas::editor::saveFileNoArgs),
//     shortcut(sf::Keyboard::Scancode::T, {sf::Keyboard::Scancode::LControl}, &tas::editor::inputSeqWithSelection::appendLines),
//     shortcut(sf::Keyboard::Scancode::I, {sf::Keyboard::Scancode::LControl}, &tas::editor::inputSeqWithSelection::insertSelected),
//     shortcut(sf::Keyboard::Scancode::Delete, {}, &tas::editor::inputSeqWithSelection::deleteSelected),
//     shortcut(sf::Keyboard::Scancode::D, {sf::Keyboard::Scancode::LControl}, &tas::editor::inputSeqWithSelection::duplicateSelected),
//     shortcut(sf::Keyboard::Scancode::Up, {}, &tas::editor::inputSeqWithSelection::swapSelectedUp),
//     shortcut(sf::Keyboard::Scancode::Down, {}, &tas::editor::inputSeqWithSelection::swapSelectedDown),
// };

int main(int argc, char* argv[])
{
    // start of GUI
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    
    std::thread log(tas::transmit::log_response, std::ref(tas::console::log_items));
    log.detach();

    window.resetGLStates();
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                    case sf::Keyboard::Scancode::P:
                    {
                        if (
                            (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                            || (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
                        )
                        {
                            // ctrl shift p pause
                            tas::transmit::sendCommand("attach()");
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::U:
                    {
                        if (
                            (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                            || (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
                        )
                        {
                            // ctrl shift u  unpause
                            tas::transmit::sendCommand("detach()");
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::Space:
                    {
                        if (
                            (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                            || (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
                        )
                        {
                            // ctrl shift space advance
                            tas::transmit::sendCommand("advanceFrames(1)");
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::C:
                    {
                        if (
                            (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                            || (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
                        )
                        {
                            // ctrl shift C cancel
                            tas::script::runCancel();
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RAlt))
                        {
                            // Alt C connect
                            tas::transmit::setUpConnection("192.168.86.228", 6000);
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::Z:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl z undo
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::Y:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl y redo
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::S:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl s save
                            tas::editor::saveFile();
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::O:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl o open file
                            tas::editor::openFile();
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::T:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl T add line
                            tas::editor::loaded_input_seq.appendLines();
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::I:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl I insert line
                            tas::editor::loaded_input_seq.insertSelected();
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::D:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl D duplicate line
                            tas::editor::loaded_input_seq.duplicateSelected();
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::Delete:
                    {
                        tas::editor::loaded_input_seq.deleteSelected();
                        break;
                    }
                    case sf::Keyboard::Scancode::F9:
                    {
                        // F9 run current
                        tas::script::run(tas::editor::loaded_input_seq);
                        break;
                    }
                    case sf::Keyboard::Scancode::F10:
                    {
                        // F10 run file
                        tas::script::runFile();
                        break;
                    }
                    case sf::Keyboard::Scancode::Escape:
                    {
                        // Esc clear popup windows
                        tas::clearFlags();
                    }
                }
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        
        tas::menu::mainLoop();

        ImGui::SetNextWindowPos(sf::Vector2u(0, LINE_HEIGHT));
        ImGui::SetNextWindowSize(MASTER_WINDOW_SIZE - sf::Vector2u(2 * CONTROL_PANEL_SIZE, LINE_HEIGHT));
        if (ImGui::Begin("master", &window_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("tabbar"))
            {
                tas::editor::mainLoop();
                tas::console::mainLoop();
                ImGui::EndTabBar();
            }

            tas::control::mainLoop();
            tas::script::mainLoop();
            tas::popup::mainLoop();
        } ImGui::End();
        ImGui::SFML::Render(window);
        
        for (auto cur_obj : pending_to_draw)
        {
            window.draw(*cur_obj);
        }
        window.display();
        pending_to_draw.clear();
    }
    ImGui::SFML::Shutdown();
}