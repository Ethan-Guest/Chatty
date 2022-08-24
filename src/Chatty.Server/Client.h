#pragma once
#include "TcpService.h"

class Client : public TcpService
{
public:
    Client();
    Client(const char* ipAddress, uint16_t port) :
        TcpService(ipAddress, port), clientMode(true)
    {
    }

    bool InitClient(); // Initialize the client

    void Run() override; // The main client loop

    void ReceiveFromServer();

private:

    bool clientMode;
    std::string userName; // Cleint username
    SOCKET broadcastSocket; // The socket used for receiving broadcast information from server
};
