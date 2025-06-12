#ifndef TAS_EDITOR_HPP
#define TAS_EDITOR_HPP

#include <string>

namespace tas
{
    namespace editor
    {
        extern const int NUM_OF_COLS;
        extern const char* const head[];

        void mainLoop();
        void saveFile(std::string filename = "");
        void openFile(std::string filename = "");
    } // namespace editor
    
} // namespace tas


#endif