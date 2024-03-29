#include "TcpService.h"

bool TcpService::Init() 
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        // TODO: Winsock startup error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "WINSOCK INITIALIZED\n";


    // Create a listening socket
    ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectionSocket == INVALID_SOCKET)
    {
        // Socket creation error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "LISTENING SOCKET CREATED\n";

    // Initialize socket address
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);
}
void TcpService::Run()
{

}

int TcpService::TcpSendMessage(SOCKET socket, const char* data, int length)
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