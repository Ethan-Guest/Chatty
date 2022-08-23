#include "Client.h"
#include <string>

bool Client::InitClient()
{
    // Initialize winsock
    if (!InitWinsock(false))
    {
        return false;
    }

    int result = connect(connectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
    if (result == SOCKET_ERROR)
    {
        std::cout << "ERROR: COULD NOT CONNECT";
    }
    else
    {
        std::cout << "CONNECTED\n";
    }
}

void Client::Run()
{
    while (clientMode)
    {
        // Get the line from the user
        std::string message;
        std::getline(std::cin, message);

        // Create a buffer with the size of the message
        uint8_t size = message.length() + 1;
        auto sendbuffer = new char[size];

        // Convert message to proper format for send
        memset(sendbuffer, 0, size);
        const char* messagechar = message.c_str();
        strcpy(sendbuffer, messagechar);

        // Check that the message is not empty
        if (std::strlen(messagechar) > 1)
        {
            TcpSendMessage(connectionSocket, (char*)&size, 1);
            TcpSendMessage(connectionSocket, sendbuffer, size);
        }
    }
}
