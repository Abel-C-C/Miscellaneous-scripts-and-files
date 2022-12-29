#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main(int argc, char *argv[]) {

        if (argc != 3) {
                std::cerr << "Invalid arguments. ./tcpclient [portnumber] [lan ip address])" << '\n';
                return 1;
        }

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
                return 1;
        }

        //int port = static_cast<int>(*argv[1]);
        //std::string ipAddress = argv[2];

        int port = 54000;
        std::string ipAddress = "127.0.0.1";


        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

        int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connectRes == -1) {
                return 1;
        }

        char buf[4096];
        std::string userInput;

        do {

                std::cout << ">> ";
                getline(std::cin, userInput);

                int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
                if (sendRes == -1) {
                        std::cout << "Message didn't reach server \r\n";
                        continue;
                }


                memset(buf, 0, 4096);
                int bytesRecieved = recv(sock, buf, 4096, 0);
                if (bytesRecieved == -1) {
                        std::cout << "Server failed to respond\r\n";
                } else {
                        std::cout << "Sent: " << std::string(buf, bytesRecieved) << "\r\n";
                }

        } while (true);

        close(sock);

        return 0;
}
