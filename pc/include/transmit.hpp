#ifndef TAS_TRANSMIT_HPP
#define TAS_TRANSMIT_HPP

#include <string>
#include <deque>

namespace tas
{
    namespace transmit
    {
        extern bool connected;
        extern bool paused;
        extern int sockfd;

        void log_response(std::deque<std::string>& log_items);
        void sendCommand(std::string cmd);
        bool setUpConnection(const char* ip, int port);
    } // namespace transmit
    
} // namespace tas


#endif