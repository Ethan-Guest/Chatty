
#include "TcpService.h"
#include "Helper.h"


bool TcpService::InitWinsock(bool clientStartup)
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        // TODO: Winsock startup error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - WINSOCK INITIALIZED\n";

    if (clientStartup)
    {
        // If the client startup mode is active, await broadcast from server
        if (!ClientBroadcastReceive())
        {
            // Client could not receive broadcast
            return false;
        }
    }

    std::vector<std::string> ipAndPort = Helper::StringToVector(broadcastReceiveMessage, ' ', false);
    ipAddress = ipAndPort[0].c_str();
    port = std::stoi(ipAndPort[1]);

    // Create a TCP listening socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectionSocket == INVALID_SOCKET)
    {
        // Socket creation error
        return false;
    }
    std::cout << ipAddress << std::endl;
    std::cout << "[" << "00:00:00" << "] - LISTENING SOCKET CREATED\n";

    ipAddress = "127.0.0.1";

    // Initialize socket address
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &socketAddress.sin_addr);

    return true;
}

void TcpService::Run()
{
    
}

bool TcpService::ClientBroadcastReceive()
{
    // Create UDP socket
    broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
    serverHint.sin_family = AF_INET; // Address format is IPv4
    serverHint.sin_port = htons(31337); // Convert from little to big endian

    // Try and bind the socket to the IP and port
    if (bind(broadcastSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in serverInfo; // Store server information (port / ip address)
    int serverLength = sizeof(serverInfo); // The size of the client information

    char receiveBuffer[16];

    std::cout << "Waiting connection from server...\n";
    // Loop until a UDP connection is established
    while (true)
    {
        // Clear the server structure
        ZeroMemory(&serverInfo, serverLength);

        // Clear the receive buffer
        ZeroMemory(receiveBuffer, 16);

        // Wait for message
        int result = recvfrom(broadcastSocket, receiveBuffer, 16, 0, (sockaddr*)&serverInfo, &serverLength);
        if (result == SOCKET_ERROR)
        {
            continue;
        }

        if (result > 0)
        {
            // Read the message

            std::cout << "Message recv" << receiveBuffer << std::endl;

            std::string message(receiveBuffer);

            broadcastReceiveMessage = message;


            closesocket(broadcastSocket);
            return true;
        }
        // Display message and client info
        //char clientIp[256]; // Convert the address byte array to string of characters
        //ZeroMemory(clientIp, 256);

        //// Convert from byte array to chars
        //inet_ntop(AF_INET, &serverInfo.sin_addr, clientIp, 256);


        // Display the message / who sent it
    }
    return false;
}


int TcpService::TcpSendMessage(SOCKET socket, const char* data, uint16_t length)
{
    int result;
    int bytesSent = 0;

    while (bytesSent < length)
    {
        result = send(socket, data + bytesSent, length - bytesSent, 0);

        if (result <= 0)
            return result;

        bytesSent += result;
    }

    return bytesSent;
}

void TcpService::TcpSendFullMessage(SOCKET s, std::string message)
{
    int32_t size = message.length() + 1;
    const char* data = message.c_str();

    // Send the size of the data 
    int result = TcpSendMessage(s, (char*)&size, 1);

    // Send the data itself
    result = TcpSendMessage(s, data, size);
}

int TcpService::TcpRecieveMessage(SOCKET socket, char* buf, int length)
{
    int total = 0;
    do
    {
        int ret = recv(socket, buf + total, length - total, 0);
        if (ret < 1)
            return ret;
        total += ret;
    } while (total < length);

    return total;
}