#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cerrno>
#include <unordered_set>
#include <thread>

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

int main(int argc, char* argv[])
{
    // start of GUI
    sf::RenderWindow window(sf::VideoMode(MASTER_WINDOW_SIZE), "Scriptease 0.1.0");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    
    std::thread log(tas::transmit::log_response, std::ref(tas::console::log_items));
    log.detach();

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
                            tas::transmit::sendCommand("pause");
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
                            tas::transmit::sendCommand("unpause");
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
                            tas::transmit::sendCommand("advance");
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
                            tas::script::appendLines(tas::script::loadedInputSeq);
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::I:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl I insert line
                            tas::script::insertLines(tas::script::loadedInputSeq);
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::D:
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
                        {
                            // ctrl D duplicate line
                            tas::script::duplicateLines(tas::script::loadedInputSeq);
                        }
                        break;
                    }
                    case sf::Keyboard::Scancode::Delete:
                    {
                        tas::script::deleteLines(tas::script::loadedInputSeq);
                    }
                    case sf::Keyboard::Scancode::F9:
                    {
                        // F9 run current
                        tas::script::run(tas::script::loadedInputSeq);
                        break;
                    }
                    case sf::Keyboard::Scancode::F10:
                    {
                        // F10 run file
                        tas::script::runFile();
                        break;
                    }
                }
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        
        tas::menu::mainLoop();
        tas::control::mainLoop();
        tas::console::mainLoop();
        tas::editor::mainLoop();
        tas::popup::mainLoop();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}