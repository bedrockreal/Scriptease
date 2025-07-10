#include "script.hpp"
#include "common.hpp"
#include "transmit.hpp"
#include "menu.hpp"
#include "console.hpp"
#include "editor.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace tas
{
    namespace script
    {
        frameInputMsg::frameInputMsg()
        {
            for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j)
                joy_pos[i][j] = 0;
            std::fill(isPressed, isPressed + 16, 0);
        }

        
        int frameInputMsg::loadNxTasStr(std::string str)
        {
            std::fill(isPressed, isPressed + 16, 0);
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
                joy_pos[0][0] = std::stoi(tmp[0]); joy_pos[0][1] = std::stoi(tmp[1]);
                tmp = tas::tokenize(argv[3], ';');
                joy_pos[1][0] = std::stoi(tmp[0]); joy_pos[1][1] = std::stoi(tmp[1]);
                return std::stoi(argv[0]);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            return 0;
        }
        frameInputMsg::~frameInputMsg() {}

        frameInputMsg frameInputMsg::clone()
        {
            auto ret = frameInputMsg();
            memcpy(ret.joy_pos[0], joy_pos[0], sizeof joy_pos[0]);
            memcpy(ret.joy_pos[1], joy_pos[1], sizeof joy_pos[1]);
            memcpy(ret.isPressed, isPressed, sizeof isPressed);
            return ret;
        }

        std::string frameInputMsg::getNxTasStr()
        {
            // frame no. not inlcuded
            std::string btns = "NONE";
            for (int j = 0; j < 16; ++j) if (isPressed[j])
            {
                if (btns == "NONE") btns = "KEY_" + std::string(proper[j]);
                else btns += ";KEY_" + std::string(proper[j]);
            }
            return btns + " " + std::to_string(joy_pos[0][0]) + ";" + std::to_string(joy_pos[0][1])
                        + " " + std::to_string(joy_pos[1][0]) + ";" + std::to_string(joy_pos[1][1]);
        }

        std::string frameInputMsg::getSysStr()
        {
            int btnState = 0;
            for (int i = 0; i < 16; ++i) if (isPressed[i])
                btnState |= (1 << i);
            return "setControllerState " + std::to_string(btnState)
                + " " + std::to_string(joy_pos[0][0]) + " " + std::to_string(joy_pos[0][1])
                + " " + std::to_string(joy_pos[1][0]) + " " + std::to_string(joy_pos[1][1]);
        }
        
        bool frameInputMsg::isIdle()
        {
            if (joy_pos[0][0] != 0 || joy_pos[0][1] != 0) return 0;
            if (joy_pos[1][0] != 0 || joy_pos[1][1] != 0) return 0;
            for (int i = 0; i < 16; ++i) if (isPressed[i]) return 0;
            return 1;
        }

        void frameInputMsg::clampJoystickCoords()
        {
            for (int i = 0; i < 2; ++i)
            {
                joy_pos[0][i] = std::max(joy_pos[0][i], -0x7fff);
                joy_pos[0][i] = std::min(joy_pos[0][i], 0x7fff);
                joy_pos[1][i] = std::max(joy_pos[1][i], -0x7fff);
                joy_pos[1][i] = std::min(joy_pos[1][i], 0x7fff);
            }
        }

        void inputSeq::appendLines(int cnt)
        {
            while (cnt-- > 0) push_back(frameInputMsg());
        }

        void inputSeq::deleteLines(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0; --j)
            {
                int i = idx[j];
                erase(begin() + i);
            }
        }

        void inputSeq::insertLines(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0; --j)
            {
                int i = idx[j];
                insert(begin() + i, frameInputMsg());
            }
        }

        void inputSeq::duplicateLines(std::vector<int> idx)
        {
            for (int j = idx.size() - 1; j >= 0;)
            {
                int r = idx[j];
                int l = r;
                while (1)
                {
                    --j;
                    if (j >= 0 && idx[j] == l - 1) --l;
                    else break;
                };
                for (int i = r; i >= l; --i)
                {
                    insert(begin() + l, (begin() + r)->clone());
                }
            }
        }

        void inputSeq::loadFromFile(std::string filename)
        {
            std::ifstream file((std::string("scripts/") + std::string(filename).c_str()));
            if (!file.is_open())
            {
                std::cerr << "Error: cannot open " << filename << std::endl;
            }
            else
            {
                clear();
                std::string line;
                while (std::getline(file, line))
                {
                    tas::script::frameInputMsg msg;
                    int cur_frame = msg.loadNxTasStr(line);
                    appendLines(cur_frame - size());
                    push_back(msg);
                }
            }
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

        inputSeq run_input_seq;
        int frame_to_run = 0;

        void mainLoop()
        {
            if (!run_input_seq.empty())
            {
                if (frame_to_run >= run_input_seq.size())
                {
                    // end, reset
                    run_input_seq.clear();
                    frame_to_run = 0;
                    transmit::sendCommand("resetControllerState()");
                }
                else
                {
                    console::log("running frame " + std::to_string(frame_to_run));
                    while (!console::log_items.empty() && console::log_items.back() == "switch: advance") console::log_items.pop_back();
                    transmit::sendCommand(run_input_seq[frame_to_run++].getSysStr());
                    transmit::sendCommand("advanceFrames(1)");
                    while (console::log_items.empty() || console::log_items.back().find("advanceFrames(1)") == std::string::npos); // wait
                    console::log_items.pop_back();
                }
            }
        }

        void run(inputSeq& m_input_seq)
        {
            run_input_seq = m_input_seq;
            frame_to_run = 0;
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
                inputSeq cur;
                cur.loadFromFile(filename);
                run(cur);
            }
        }

        void runCancel()
        {
            run_input_seq.clear();
            frame_to_run = 0;
            transmit::sendCommand("resetControllerState()");
            transmit::sendCommand("detach()");
        }
    }
}

// std::vector<uint8_t> getBytes(frameInputMsg& frame_input_msg)
// {
//     uint8_t* bytePtr = reinterpret_cast<uint8_t*>(&frame_input_msg);
//     return std::vector<uint8_t>(bytePtr, bytePtr + sizeof(frame_input_msg));
// }