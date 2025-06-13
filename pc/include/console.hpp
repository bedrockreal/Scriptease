#ifndef TAS_CONSOLE_HPP
#define TAS_CONSOLE_HPP

#include <deque>
#include <string>

namespace tas
{
    namespace console
    {
        extern bool focusHere;
        extern char input[64];
        extern std::deque<std::string> log_items;
        void mainLoop();
        void log(std::string str);
        void parse(std::string str);
    } // namespace console
    
} // namespace tas

#endif