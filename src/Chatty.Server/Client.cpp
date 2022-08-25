#include "Client.h"
#include <string>

bool Client::InitClient()
{
    // Initialize winsock
    if (!InitWinsock(true))
    {
        return false;
    }

    // Connect to server
    int result = connect(connectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
    if (result == SOCKET_ERROR)
    {
        std::cout << "ERROR: COULD NOT CONNECT\n";
        closesocket(connectionSocket);
        WSACleanup();
        return false;
    }
    std::cout << "[STARTUP] - CONNECTED TO SERVER\n\nType $help to get started.\n";
    return true;
}

void Client::Run()
{
    // Create a thread for reading user input
    std::thread readInputThread(&Client::ReceiveFromServer, this);

    while (clientMode)
    {
        while (run.load())
        {
            // Get the line from the user
            std::string message;
            std::getline(std::cin, message);

            // Create a buffer with the size of the message
            uint8_t size = message.length() + 1;
            auto sendbuffer = new char[size];

            // Convert message to proper format for send
            ZeroMemory(sendbuffer, size);
            //memset(sendbuffer, 0, size);
            const char* messagechar = message.c_str();
            strcpy(sendbuffer, messagechar);

            // Check that the message is not empty
            if (std::strlen(messagechar) > 0)
            {
                int result = TcpSendMessage(connectionSocket, (char*)&size, 1);
                result = TcpSendMessage(connectionSocket, sendbuffer, size);
            }
        }
    }

    // Close the thread
    run.store(false);
    readInputThread.join();

    // Close the connection
    shutdown(connectionSocket, SD_BOTH);
    closesocket(connectionSocket);
    closesocket(broadcastSocket);

    WSACleanup();
}

void Client::ReceiveFromServer()
{
    while (run.load())
    {
        // Receive the size of the data
        int result = TcpRecieveMessage(connectionSocket, (char*)&messageSize, 1);

        // Create the buffer
        buffer = new char[messageSize];

        // Receive the data itself
        result = TcpRecieveMessage(connectionSocket, buffer, messageSize);
        //	If error appeared during receipt and WAS caused by shutdown, return SHUTDOWN.
        if (result == 0)
        {
            std::cout << "DISCONNCTED FROM SERVER.\n";
            run = false;
            clientMode = false;
            continue;
        }
        //	If error appeared during receipt and was not caused by shutdown, return DISCONNECT.
        if (result == SOCKET_ERROR)
        {
            std::cout << "The server was shutdown.\n";
            run = false;
            continue;
        }

        // Write the received message to console and clear the buffer for re-use
        std::cout << buffer << "\n";
        ZeroMemory(buffer, messageSize);
    }
}
