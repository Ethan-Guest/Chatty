#include "Client.h"
#include <string>
#include <atomic>
#include <thread>

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
        std::cout << "ERROR: COULD NOT CONNECT\n";
        closesocket(connectionSocket);
        WSACleanup();
        return false;
    }
    else
    {
        std::cout << "CONNECTED TO SERVER\n";
    }
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
            memset(sendbuffer, 0, size);
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
    shutdown(connectionSocket, SD_BOTH);
    closesocket(connectionSocket);
    // Main loop listens for messages from server
    //while (run.load())
    //{
    //    //std::cout << "test\n";
    //    //std::this_thread::sleep_for(std::chrono::seconds(3));

    //    int result = TcpRecieveMessage(connectionSocket, (char*)&messageSize, 1);
    //    result = TcpRecieveMessage(connectionSocket, (char*)buffer, messageSize);

    //}
    //run.store(false)
    //readInputThread.join();
    //ReadInputLoop();
}

void Client::ReceiveFromServer()
{
    while (run.load())
    {
        uint8_t size = 0;
        auto msgBuffer = new char[size];
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        //std::cout << "TEST\n";
        int result = TcpRecieveMessage(connectionSocket, (char*)&size, 1);
        result = TcpRecieveMessage(connectionSocket, msgBuffer, size);
        std::cout << msgBuffer;
        //ZeroMemory(msgBuffer, size);


    }

}
