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

    void Run() override; // The main client loop

    void ReceiveFromServer(); // Receive data from the server

private:
    bool clientMode; // Used for winsock startup, indicates that a UDP connection 
    //must be received before attempting TCP connection

    std::string userName; // Cleint username
};
