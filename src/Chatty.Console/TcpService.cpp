#include "TcpService.h"
#include "Helper.h"


bool TcpService::InitWinsock(bool clientStartup)
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        std::cout << "[STARTUP] - ERROR: WINSOCK COULD NOT START\n";
        return false;
    }
    std::cout << "[STARTUP] - WINSOCK INITIALIZED\n";

    // If the client startup mode is active, await broadcast from server
    if (clientStartup)
    {
        if (!ClientBroadcastReceive())
        {
            // Client could not receive broadcast
            std::cout << "[STARTUP] - ERROR: ERROR RECEIVING TCP INFORMATION FROM SERVER\n";
            return false;
        }
    }

    // Get ip and port from broadcast message (client only)
    std::vector<std::string> ipAndPort = Helper::StringToVector(broadcastReceiveMessage, ' ', false);
    if (clientStartup)
    {
        ipAddress = ipAndPort[0].c_str();
        port = std::stoi(ipAndPort[1]);
    }


    // Create a TCP listening socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectionSocket == INVALID_SOCKET)
    {
        // Socket creation error
        return false;
    }
    std::cout << "[STARTUP] - LISTENING SOCKET CREATED\n";

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

    // UDP socket structure
    sockaddr_in clientAddr;
    clientAddr.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
    clientAddr.sin_family = AF_INET; // Address format is IPv4
    clientAddr.sin_port = htons(31337); // Convert from little to big endian

    // Try to bind the socket to the IP and port
    if (bind(broadcastSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
    {
        std::cout << "[STARTUP] - ERROR: COULD NOT BIND UDP SOCKET\n";
        return false;
    }

    // Structure for UDP server connection
    sockaddr_in serverInfo;
    int serverLength = sizeof(serverInfo);

    // Create a buffer for receiving message from server
    char receiveBuffer[16];

    std::cout << "[STARTUP] - WAITING FOR SERVER...\n";

    // Loop until a UDP connection is established
    while (true)
    {
        // Clear the server structure
        ZeroMemory(&serverInfo, serverLength);

        // Clear the receive buffer
        ZeroMemory(receiveBuffer, 16);

        // Wait for message from broadcast socket
        int result = recvfrom(broadcastSocket, receiveBuffer, 16, 0, (sockaddr*)&serverInfo, &serverLength);
        if (result == SOCKET_ERROR)
        {
            std::cout << "[STARTUP] - ERROR: COULD NOT RECEIVE MESSAGE FROM SERVER\n";
            continue;
        }

        if (result > 0)
        {
            // Read the message
            std::string message(receiveBuffer);
            std::cout << "[STARTUP] - TCP INFORMATION RECEIVED FROM SERVER BROADCAST: " << message << "\n";
            broadcastReceiveMessage = message;

            // Close the UDP socket, allowing other clients to connect
            closesocket(broadcastSocket);
            return true;
        }
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
    }
    while (total < length);

    return total;
}
