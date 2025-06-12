#ifndef TAS_TRANSMIT_HPP
#define TAS_TRANSMIT_HPP

#include <iostream>
#include <string>
#include <deque>

#include <sys/socket.h>
#include <arpa/inet.h>

namespace tas
{
    namespace transmit
    {
        extern bool connected;
        extern bool paused;
        extern int sockfd;

        void log_response(std::deque<std::string>& log_items);
        void sendCommand(std::string cmd);
        bool setUpConnection(std::string ip, int port);
    } // namespace transmit
    
} // namespace tas


#endif