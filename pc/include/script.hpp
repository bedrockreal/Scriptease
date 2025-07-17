#ifndef TAS_SCRIPT_HPP
#define TAS_SCRIPT_HPP

#include "imgui.h"

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>

namespace tas
{
    namespace script
    {
        struct frameInputMsg
        {
            int joy_pos[2][2];
            bool isPressed[16];
            frameInputMsg();
            ~frameInputMsg();
            frameInputMsg clone();
            int loadNxTasStr(std::string str);
            std::string getNxTasStr();
            std::string getSysStr();
            bool isIdle();
            void clampJoystickCoords();
        };

        struct inputSeq : public std::vector<frameInputMsg>
        {
            virtual void appendLines(int cnt = 1);
            void insertLines(std::vector<int> idx);
            void deleteLines(std::vector<int> idx);
            void duplicateLines(std::vector<int> idx);
            virtual void loadFromFile(std::string filename);
        };

        extern const char* const proper[16];
        extern const std::unordered_map<std::string, int> translate_keys;

        void runCancel();
    }
} // namespace tas

#endif