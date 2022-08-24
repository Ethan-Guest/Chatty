#include "Client.h"
#include <string>
#include <Helper.h>

bool Client::InitClient()
{
    // Initialize winsock
    if (!InitWinsock(false))
    {
        return false;
    }

    // Listen for UDP broadcast message

    // Create UDP socket
    //broadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Client UDP connection
    //sockaddr_in clientAddress;
    //clientAddress.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
    //clientAddress.sin_family = AF_INET; // Address format is IPv4
    //clientAddress.sin_port = htons(31337); // Convert from little to big endian

    // Try and bind the socket to the IP and port
    //if (bind(broadcastSocket, (sockaddr*)&clientAddress, sizeof(clientAddress)) == SOCKET_ERROR)
    //{
    //    std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
    //    return false;
    //}

    //sockaddr_in serverInfo; // Store server information (port / ip address)
    //int serverLength = sizeof(serverInfo); // The size of the client information

    //char receiveBuffer[16];

    //std::cout << "[WAITING FOR CONNECTION FROM SERVER...]\n";

    //// Loop until a UDP connection is established
    //while (true)
    //{
    //    // Clear the server structure
    //    ZeroMemory(&serverInfo, serverLength);

    //    // Clear the receive buffer
    //    ZeroMemory(receiveBuffer, 16);

    //    // Wait for message
    //    int result = recvfrom(broadcastSocket, receiveBuffer, 16, 0, (sockaddr*)&serverInfo, &serverLength);
    //    if (result == SOCKET_ERROR)
    //    {
    //        continue;
    //    }
    //    if (result > 0)
    //    {
    //        // Read the TCP information (ip and port)
    //        auto TCPInfo = Helper::CharPtrToVector(receiveBuffer, ' ', false);

    //        // Set the ip address and port for TCP connection
    //        tcpIpAddress = TCPInfo[0].c_str();
    //        port = stoi(TCPInfo[1]);

    //        std::cout << "[TCP INFOMRATION RECEIVED]\n";

    //        // Clear the buffer
    //        ZeroMemory(receiveBuffer, 16);

    //        // Close the broadcast socket
    //        closesocket(broadcastSocket);
    //        break;
    //    }
    //}

    // Set the TCP socket port and address
    //socketAddress.sin_port = htons(port);
    //socketAddress.sin_addr.s_addr = inet_addr(tcpIpAddress);


    int result = connect(connectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
    if (result == SOCKET_ERROR)
    {
        std::cout << "[ERROR: COULD NOT CONNECT]\n";
        closesocket(connectionSocket);
        WSACleanup();
        return false;
    }
    else
    {
        std::cout << "[CONNECTED TO SERVER]\nType $help to get started.\n";
    }
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
            std::cout << "[DISCONNCTED FROM SERVER]\n";
            run = false;
            continue;
        }
        //	If error appeared during receipt and was not caused by shutdown, return DISCONNECT.
        if (result == SOCKET_ERROR)
        {
            std::cout << "[ERROR: THE SERVER WAS SHUTDOWN]\n";
            //run = false;
            continue;
        }

        std::cout << buffer << "\n";
        ZeroMemory(buffer, messageSize);
    }
}
