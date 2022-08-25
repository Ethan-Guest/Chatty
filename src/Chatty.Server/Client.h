#pragma once
#include "TcpService.h"

class Client : public TcpService
{
public:
    Client() :
        TcpService(), clientMode(true)
    {
    }

    bool InitClient(); // Initialize the client

    bool AwaitUdpBroadcast();

    void Run() override; // The main client loop

    void ReceiveFromServer();

private:
    const char* tcpIpAddress;
    bool clientMode;
    std::string userName; // Cleint username
};
