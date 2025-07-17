#include "common.hpp"

namespace tas
{
    bool showSaveFileWindow_Flag    = false;
    bool showOpenFileWindow_Flag    = false;
    bool showRunFileWindow_Flag     = false;
    bool showInputIPWindow_Flag     = false;
    std::vector<std::string> tokenize(std::string input, char delim)
    {
        std::stringstream ss(input);
        std::vector<std::string> res;
        std::string cur;
        while (std::getline(ss, cur, delim)) res.push_back(cur);
        return res;
    }

    void clearFlags()
    {
        showOpenFileWindow_Flag = 0;
        showSaveFileWindow_Flag = 0;
        showRunFileWindow_Flag = 0;
        showInputIPWindow_Flag = 0;
    }
} // namespace tas