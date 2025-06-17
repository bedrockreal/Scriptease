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
            int joyL[2];
            int joyR[2];
            bool isPressed[16];
            frameInputMsg();
            ~frameInputMsg();
            frameInputMsg clone();
            int loadNxTasStr(std::string str);
            std::string getNxTasStr();
            std::string getSysStr();
            bool isIdle();
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
        extern inputSeq run_input_seq;
        extern int frame_to_run;
        void mainLoop();
        void run(inputSeq& m_input_seq);
        void runFile(std::string filename = "");
        void runCancel();
    }
} // namespace tas

#endif