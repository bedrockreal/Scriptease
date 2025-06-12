#ifndef TAS_SCRIPT_HPP
#define TAS_SCRIPT_HPP

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
            bool isSelected;
            frameInputMsg();
            ~frameInputMsg();
            int loadNxTasStr(std::string str);
            std::string getNxTasStr();
            std::string getSysStr();
            bool isIdle();
        };

        extern const char* const proper[16];
        extern const std::unordered_map<std::string, int> translate_keys;
        extern std::vector<frameInputMsg> loadedInputSeq;
        void loadFromFile(std::string filename, std::vector<frameInputMsg>& tar);
        void appendLines(std::vector<frameInputMsg>& cur, int cnt = 1);
        void insertLines(std::vector<frameInputMsg>& cur);
        void deleteLines(std::vector<frameInputMsg>& cur);
        void duplicateLines(std::vector<frameInputMsg>& cur);
        void run(std::vector<frameInputMsg>& m_inputSeq);
        void runFile(std::string filename = "");
    }
} // namespace tas

#endif