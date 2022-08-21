/*
#include "Server.h"

bool Server::Init()
{
    // Initialize winsock
    WSADATA wsadata;
    if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    {
        // TODO: Winsock startup error
        
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "STARTUP SUCCESSFUL\n";

    // Create a listening socket
    listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSocket == INVALID_SOCKET)
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

    // Bind the socket to the ip address
    if (bind(listeningSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        // Binding error
        
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "LISTENING SOCKET BINDING SUCCESSFUL\n";
    
    // Start listening on the server socket
    if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        // Listening error

        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "LISTENING\n";
    
    // Add listening socket to master set
    FD_ZERO(&masterSet);
    FD_ZERO(&readySet);
    FD_SET(listeningSocket, &masterSet);
    return true;
}
// Server loop
void Server::Run()
{
    while (serverLoop)
    {
        // Copy original set
        readySet = masterSet;

        // 1 second timeout
        timeout.tv_sec = 1;

        // Select incoming sockets
        int readySockets = select(0, &readySet, nullptr, nullptr, &timeout);

        if (FD_ISSET(listeningSocket, &readySet))
        {
            // TODO:
        }

        // Loop through ready sockets
        for (int i = 0; i < readySockets; i++)
        {
            SOCKET communicationSocket = readySet.fd_array[i];
            if (communicationSocket == listeningSocket)
            {
                // Accept a new connection
                SOCKET client = accept(listeningSocket, nullptr, nullptr);

                // add the new connection to the list of connected clients
                FD_SET(client, &masterSet);

                std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "CLIENT " << client << " CONNECTED\n";

                // TODO: Handle client connection stuff
            }
            else
            {
                // accept new message
                int bytes = Recv(communicationSocket, (char*)&messageSize, 1);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "SERVER: " << "CLIENT " << communicationSocket <<
                        " DISCONNECTED\n";

                    // Drop the client
                    shutdown(communicationSocket, SD_BOTH);
                    closesocket(communicationSocket);
                    FD_CLR(communicationSocket, &masterSet);
                    continue;
                }

                bytes = Recv(communicationSocket, buffer, messageSize);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "MESSAGE COULD NOT BE RECEIVED\n";
                }
                else
                {
                    // Read message contents
                    std::cout << "[" << "00:00:00" << "] - " << "CLIENT " << communicationSocket << ": " << buffer <<
                        "\n";
                }

            }
        }
    }
}


int Server::Send(SOCKET skSocket, const char* data, uint16_t length)
{
    int result;
    int bytesSent = 0;

    while (bytesSent < length)
    {
        result = send(skSocket, data + bytesSent, length - bytesSent, 0);

        if (result <= 0)
            return result;

        bytesSent += result;
    }

    return bytesSent;
}

int Server::Recv(SOCKET s, char* buf, int len)
{
    int total = 0;

    do
    {
        int ret = recv(s, buf + total, len - total, 0);
        if (ret < 1)
            return ret;
        total += ret;
    }
    while (total < len);

    return total;
}

void Server::Cleanup()
{
}

SOCKET Server::Create_Socket()
{
    return 0;
}

SOCKET Server::Net_Multiplexer()
{
    return 0;
}
*/
