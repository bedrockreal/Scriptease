#include "transmit.hpp"

namespace tas
{
    namespace transmit
    {
        bool connected = 0;
        bool paused = 0;
        int sockfd;

        void log_response(std::deque<std::string>& log_items)
        {
            char cur[128] = {0};
            while (1)
            {
                recv(sockfd, cur, 128, 0);
                if (cur[0]) log_items.push_back(cur);
                // std::cout << cur << std::endl;
            }
        }

        void sendCommand(std::string cmd)
        {
            send(sockfd, (cmd + "\r\n").c_str(), cmd.length() + 2, 0);
        }

        bool setUpConnection(std::string ip, int port)
        {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1)
            {
                std::cout << "could not create TCP listening socket" << std::endl;
                return 0;
            }
            
            sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(port);
            int status = inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);
            if (status <= 0)
            {
                perror("Invalid address/ Address not supported \n");
                return 0;
            }
            if ((status = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
                printf("\nConnection Failed \n");
                return 0;
            }
            std::cout << "Connected" << std::endl;
            connected = 1;
            return 1;
        }
    } // namespace transmit
    
} // namespace tas
