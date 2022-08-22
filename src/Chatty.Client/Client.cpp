#include "Client.h"

bool Client::InitClient()
{
    // Initialize winsock
    if (!InitWinsock())
    {
        return false;
    }

    int result = connect(connectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
    std::cout << "CONNECTED";
}

void Client::Run()
{
    while (clientMode)
    {
        std::string input;
        std::cin >> input;
        TcpSendFullMessage(connectionSocket, input);
    }
}