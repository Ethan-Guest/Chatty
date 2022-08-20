#include "Server.h"

bool Server::InitServer()
{
    // Initialize winsock
    if (!InitWinsock())
    {
        return false;
    }

    // Bind the socket to the ip address
    if (bind(ConnectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR)
    {
        // Binding error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "LISTENING SOCKET BINDING SUCCESSFUL\n";

    // Start listening on the server socket
    if (listen(ConnectionSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        // Listening error
        return false;
    }

    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "LISTENING\n";

    // Add listening socket to master set
    FD_ZERO(&masterSet);
    FD_ZERO(&readySet);
    FD_SET(ConnectionSocket, &masterSet);

    return true;
}

void Server::Run()
{
}

void Server::ParseCommand()
{
}

void Server::OnClientConnect()
{
}

void Server::OnClientDisconnect()
{
}
