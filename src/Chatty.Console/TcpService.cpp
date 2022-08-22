
#include "TcpService.h"


bool TcpService::InitWinsock() 
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        // TODO: Winsock startup error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - WINSOCK INITIALIZED\n";


    // Create a listening socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectionSocket == INVALID_SOCKET)
    {
        // Socket creation error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - LISTENING SOCKET CREATED\n";

    // Initialize socket address
    socketAddress.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddress, &socketAddress.sin_addr);
    socketAddress.sin_port = htons(port);

    return true;
}

void TcpService::Run()
{
    
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
    int32_t size = message.length();
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