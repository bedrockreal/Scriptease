#include "script.hpp"
#include "common.hpp"
#include "transmit.hpp"
#include "menu.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace tas
{
    namespace script
    {
        frameInputMsg::frameInputMsg()
        {
            joyL[0] = joyL[1] = 0;
            joyR[0] = joyR[1] = 0;
            std::fill(isPressed, isPressed + 16, 0);
            isSelected = 0;
        }

        
        int frameInputMsg::loadNxTasStr(std::string str)
        {
            std::fill(isPressed, isPressed + 16, 0);
            isSelected = 0;
            auto argv = tas::tokenize(str, ' ');
            try
            {
                if (argv[1] != "NONE")
                {
                    auto keys_pressed = tokenize(argv[1], ';');
                    for (auto& key : keys_pressed)
                    {
                        key = key.substr(4, key.length() - 4); // remove "KEY_"
                        isPressed[std::__lg(tas::script::translate_keys.at(key))] = 1;
                    }
                }
                auto tmp = tas::tokenize(argv[2], ';');
                joyL[0] = std::stoi(tmp[0]); joyL[1] = std::stoi(tmp[1]);
                tmp = tas::tokenize(argv[3], ';');
                joyR[0] = std::stoi(tmp[0]); joyR[1] = std::stoi(tmp[1]);
                return std::stoi(argv[0]);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            return 0;
        }
        frameInputMsg::~frameInputMsg() {}

        std::string frameInputMsg::getNxTasStr()
        {
            // frame no. not inlcuded
            std::string btns = "NONE";
            for (int j = 0; j < 16; ++j) if (isPressed[j])
            {
                if (btns == "NONE") btns = "KEY_" + std::string(proper[j]);
                else btns += ";KEY_" + std::string(proper[j]);
            }
            return btns + " " + std::to_string(joyL[0]) + ";" + std::to_string(joyL[1])
                        + " " + std::to_string(joyR[0]) + ";" + std::to_string(joyR[1]);
        }

        std::string frameInputMsg::getSysStr()
        {
            int btnState = 0;
            for (int i = 0; i < 16; ++i) if (isPressed[i])
                btnState |= (1 << i);
            return "setControllerState " + std::to_string(btnState)
                + " " + std::to_string(joyL[0]) + " " + std::to_string(joyL[1])
                + " " + std::to_string(joyR[0]) + " " + std::to_string(joyR[1]);
        }
        
        bool frameInputMsg::isIdle()
        {
            if (joyL[0] != 0 || joyL[1] != 0) return 0;
            if (joyR[0] != 0 || joyR[1] != 0) return 0;
            for (int i = 0; i < 16; ++i) if (isPressed[i]) return 0;
            return 1;
        }

        const char* const proper[16] =
        {
            "A\0",
            "B\0",
            "X\0", 
            "Y\0",
            "LSTICK\0", 
            "RSTICK\0",
            "L\0",
            "R\0",
            "ZL\0",
            "ZR\0",
            "PLUS\0",
            "MINUS\0",
            "DLEFT\0",
            "DUP\0",
            "DRIGHT\0",
            "DDOWN\0"
        };

        const std::unordered_map<std::string, int> translate_keys =
        {
            {"NONE", 0},
            {"A", 1 << 0},
            {"B", 1 << 1},
            {"X", 1 << 2},
            {"Y", 1 << 3},
            {"LSTICK", 1 << 4},
            {"RSTICK", 1 << 5},
            {"L", 1 << 6},
            {"R", 1 << 7},
            {"ZL", 1 << 8},
            {"ZR", 1 << 9},
            {"PLUS", 1 << 10},
            {"MINUS", 1 << 11},
            {"DLEFT", 1 << 12},
            {"DUP", 1 << 13},
            {"DRIGHT", 1 << 14},
            {"DDOWN", 1 << 15}
        };

        std::vector<frameInputMsg> editorInputSeq;
        std::vector<frameInputMsg> tmpInputSeq;
        int frameToRun = 0;

        void mainLoop()
        {
            if (!tmpInputSeq.empty())
            {
                transmit::sendCommand(tmpInputSeq[frameToRun++].getSysStr());
                transmit::sendCommand("advance");
                if (frameToRun == tmpInputSeq.size())
                {
                    // end, reset
                    tmpInputSeq.clear();
                    frameToRun = 0;
                }
            }
        }


        void loadFromFile(std::string filename, std::vector<frameInputMsg>& tar)
        {
            std::ifstream file((std::string("scripts/") + std::string(filename).c_str()));
            if (!file.is_open())
            {
                std::cerr << "Error: cannot open " << filename << std::endl;
            }
            else
            {
                tar.clear();
                std::string line;
                while (std::getline(file, line))
                {
                    tas::script::frameInputMsg msg;
                    int cur_frame = msg.loadNxTasStr(line);
                    appendLines(editorInputSeq, cur_frame - editorInputSeq.size());
                    tar.push_back(msg);
                }
            }
        }

        void insertLines(std::vector<frameInputMsg>& cur)
        {
            // insert above all selected
            for (int i = 0; i < cur.size(); ++i)
            {
                if (cur[i].isSelected)
                {
                    cur.insert(cur.begin() + i, frameInputMsg());
                    ++i;
                }
            }
        }

        void appendLines(std::vector<frameInputMsg>& cur, int cnt)
        {
            while (cnt-- > 0) cur.push_back(frameInputMsg());
        }

        void deleteLines(std::vector<frameInputMsg>& cur)
        {
            for (int i = 0; i < cur.size();)
            {
                if (cur[i].isSelected) cur.erase(cur.begin() + i);
                else ++i;
            }
        }

        void duplicateLines(std::vector<frameInputMsg>& cur)
        {
            for (int i = cur.size() - 1; i >= 0; --i)
            {
                if (cur[i].isSelected)
                {
                    int j = i - 1;
                    for (; j >= 0 && cur[j].isSelected; --j);
                    ++j;
                    for (int cnt = i; cnt >= j; --cnt)
                    {
                        cur.insert(cur.begin() + j, cur[i]);
                        cur[j].isSelected = 0;
                    }
                    i = j;
                }
            }
        }

        void run(std::vector<frameInputMsg>& m_inputSeq)
        {
            tmpInputSeq = m_inputSeq;
            // for (int i = 0; i < m_inputSeq.size(); ++i)
            // {
            //     transmit::sendCommand(m_inputSeq[i].getSysStr());
            //     transmit::sendCommand("advance");
            // }
            // // reset
            // transmit::sendCommand("resetControllerState");
        }

        void runFile(std::string filename)
        {
            if (filename.empty())
            {
                // Show pop-up
                showRunFileWindow_Flag = true;
            }
            else
            {
                std::vector<frameInputMsg> cur;
                loadFromFile(filename, cur);
                run(cur);
            }
        }
    }
}

// std::vector<uint8_t> getBytes(frameInputMsg& frame_input_msg)
// {
//     uint8_t* bytePtr = reinterpret_cast<uint8_t*>(&frame_input_msg);
//     return std::vector<uint8_t>(bytePtr, bytePtr + sizeof(frame_input_msg));
// }