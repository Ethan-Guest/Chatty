#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>

// loop send func
int tcp_send_whole(SOCKET skSocket, const char* data, uint16_t length)
{
    int result;
    int bytesSent = 0;

    while (bytesSent < length)
    {
        result = send(skSocket, data + bytesSent, length - bytesSent, 0);

        if (result <= 0)
            return result;

        bytesSent += result;
    }

    return bytesSent;
}

// loop recv func
int tcp_recv_whole(SOCKET s, char* buf, int len)
{
    int total = 0;

    do
    {
        int ret = recv(s, buf + total, len - total, 0);
        if (ret < 1)
            return ret;
        total += ret;
    }
    while (total < len);

    return total;
}

int main()
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        // Winsock startup error
    }

    std::cout << "[" << "00:00:00" << "]" << " STARTUP SUCCESSFUL\n";

    // Create a listening socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        // Socket creation error
    }

    std::cout << "[" << "00:00:00" << "]" << " LISTENING SOCKET CREATED\n";

    // Initialize socket address
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAddr.sin_port = htons(31337);

    // Bind
    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        // Binding error
    }

    std::cout << "[" << "00:00:00" << "]" << " LISTENING SOCKET BINDING SUCCESSFUL\n";


    // Listen
    if (listen(listenSocket, 1) != 0)
    {
        // Listening error
    }

    std::cout << "[" << "00:00:00" << "]" << " SERVER IS LISTENING\n";

    FD_SET masterSet, readySet;
    FD_ZERO(&masterSet);
    FD_SET(listenSocket, &masterSet);
    FD_ZERO(&readySet);


    while (true)
    {
        readySet = masterSet;

        int readySockets = select(0, &readySet, nullptr, nullptr, nullptr);

        for (int i = 0; i < readySockets; i++)
        {
            SOCKET communicationSocket = readySet.fd_array[i];
            if (communicationSocket == listenSocket)
            {
                // accept a new connection
                SOCKET client = accept(listenSocket, nullptr, nullptr);

                // add the new connection to the list of connected clients
                FD_SET(client, &masterSet);

                std::cout << "[" << "00:00:00" << "]" << " NEW CLIENT CONNECTION\n";

                // send feedback to client
                uint8_t size = 24;
                char sendbuffer[24];
                memset(sendbuffer, 0, 24);
                strcpy(sendbuffer, "[CONNECTED TO SERVER]: ");

                int result = tcp_send_whole(client, (char*)&size, 1);

                result = tcp_send_whole(client, sendbuffer, size);
            }
            else
            {
                uint8_t messageSize = 0;

                auto buffer = new char[messageSize];

                // TODO: DUPLICATE CODE, COMPACT INTO ONE FUNCTION WITH SIZE PARAMETER
                // accept new message
                int bytes = tcp_recv_whole(communicationSocket, (char*)&messageSize, 1);
                if (bytes <= 0)
                {
                    // Drop the client
                    closesocket(communicationSocket);
                    FD_CLR(communicationSocket, &masterSet);
                    std::cout << "[" << "00:00:00" << "]" << " CLIENT " << communicationSocket << " DISCONNECTED\n";
                    continue;
                }

                bytes = tcp_recv_whole(communicationSocket, buffer, messageSize);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "]" << " MESSAGE COULD NOT BE RECEIVED\n";
                }
                else
                {
                    std::cout << communicationSocket << buffer << "\n";
                }
            }
        }
    }

    WSACleanup();

    ExitProcess(EXIT_SUCCESS);
}
