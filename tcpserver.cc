#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main() {

        int listening = socket(AF_INET, SOCK_STREAM, 0);
        if (listening == -1) {
                std::cerr << "Can't create a socket" << '\n';
                return -1;
        }

        sockaddr_in hint;
        hint.sin_family = AF_INET;

        //port number to accept connections     
        hint.sin_port = htons(54321);
        //server lan ip address
        inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

        if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == - 1) {
                std::cerr << "Can't bind to IP/port" << '\n';
                return -2;
        }

        if (listen(listening, SOMAXCONN) == -1) {
                std::cerr << "Can't listen" << '\n';
                return -3;
        }

        sockaddr_in client;
        socklen_t clientsize = sizeof(client);
        char host[NI_MAXHOST];
        char svc[NI_MAXSERV];

        int clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

        if (clientSocket == -1) {
                std::cerr << "Problem with client connecting" << '\n';
                return -4;
        }

        close(listening);

        memset(host, 0, NI_MAXHOST);
        memset(svc, 0, NI_MAXSERV);

        int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

        if (result) {
                std::cout << host << " connected on " << svc << '\n';
        } else {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                std::cout << host << " connected on " << ntohs(client.sin_port) << '\n';
        }

        char buf[4096];
        while (true) {
                memset(buf, 0, 4096);
                int bytesRecv = recv(clientSocket, buf, 4096, 0);
                if (bytesRecv == -1) {
                        std::cerr << "There was a connection issue" << '\n';
                        break;
                }
                if (bytesRecv == 0) {
                        std::cout << "The client disconnected" << '\n';
                        break;
                }

                std::cout << "Recieved message: " << std::string(buf, 0, bytesRecv) << '\n';
                send(clientSocket, buf, bytesRecv + 1, 0);
        }

        close(clientSocket);

        return 0;
}
