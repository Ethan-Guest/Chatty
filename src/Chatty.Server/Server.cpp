#include "Server.h"

bool Server::InitServer()
{
    // Initialize winsock
    if (!InitWinsock())
    {
        return false;
    }

    // Bind the socket to the ip address
    if (bind(connectionSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR)
    {
        // Binding error
        return false;
    }
    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "LISTENING SOCKET BINDING SUCCESSFUL\n";

    // Start listening on the server socket
    if (listen(connectionSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        // Listening error
        return false;
    }

    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "LISTENING\n";

    // Add listening socket to master set
    FD_ZERO(&masterSet);
    FD_ZERO(&readySet);
    FD_SET(connectionSocket, &masterSet);

    return true;
}

void Server::Run()
{
    while (serverLoop)
    {
        // Copy original set
        readySet = masterSet;

        // 1 second timeout
        TIMEVAL	timeout;
        timeout.tv_sec = 1;

        // Select incoming sockets
        int readySockets = select(0, &readySet, nullptr, nullptr, &timeout);



        // Loop through ready sockets
        for (int i = 0; i < readySockets; i++)
        {
            SOCKET communicationSocket = readySet.fd_array[i];

            if (communicationSocket == connectionSocket)
            {
                OnClientConnect();

            }
            else
            {
                // Accept new message

                // Set the buffer size
                buffer = new char[messageSize]; 

                // Receive the size of the message
                int bytes = TcpRecieveMessage(communicationSocket, (char*)&messageSize, 1);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "CLIENT " << communicationSocket <<
                        " DISCONNECTED\n";

                    // Drop the client
                    shutdown(communicationSocket, SD_BOTH);
                    closesocket(communicationSocket);
                    FD_CLR(communicationSocket, &masterSet);
                }

                bytes = TcpRecieveMessage(communicationSocket, buffer, messageSize);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "MESSAGE COULD NOT BE RECEIVED\n";
                }
                else
                {
                    // Read message contents
                    std::cout << "[" << "00:00:00" << "] - " << "CLIENT " << communicationSocket << ":  " << buffer <<
                        "\n";
                }

            }
        }
    }
}

void Server::ParseCommand()
{

}

void Server::OnClientConnect()
{
    // Accept a new connection
    SOCKET client = accept(connectionSocket, nullptr, nullptr);

    // add the new connection to the list of connected clients
    FD_SET(client, &masterSet);

    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "CLIENT " << client << " CONNECTED\n";
}

void Server::OnClientDisconnect()
{

}
void Server::LogAction(int mode, char* message) 
{
    //std::cout << "[00:00:00] - " << "SERVER:      " << "CLIENT " << client << " CONNECTED\n";
}