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
    while (serverMode)
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
            activeSocket = &communicationSocket;

            if (communicationSocket == connectionSocket)
            {
                // Accept a new connection
                SOCKET client = accept(connectionSocket, nullptr, nullptr);

                // add the new connection to the list of connected clients
                FD_SET(client, &masterSet);

                OnClientConnect(client);
            }
            else
            {
                // Accept new message

                // Set the buffer size
                buffer = new char[messageSize]; 

                // Receive the size of the message
                int bytes = TcpRecieveMessage(communicationSocket, (char*)&messageSize, 1);

                // If receive failed
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "CLIENT " << communicationSocket <<
                        " DISCONNECTED\n";

                    // Drop the client
                    shutdown(communicationSocket, SD_BOTH);
                    closesocket(communicationSocket);
                    FD_CLR(communicationSocket, &masterSet);

                    // Clear the buffer and continue
                    ZeroMemory(buffer, messageSize);
                    continue;
                }

                // Receive the incoming data
                bytes = TcpRecieveMessage(communicationSocket, buffer, messageSize);
                if (bytes <= 0)
                {
                    std::cout << "[" << "00:00:00" << "] - " << "MESSAGE COULD NOT BE RECEIVED\n";
                }
                if (buffer[0] == '$')
                {
                    ReadCommand();
                }
                // If the user is registered, read the message. 
                else if (clients.at(communicationSocket)->isRegistered == true)
                {
                    LogAction(buffer);
                    std::cout << "[" << "00:00:00" << "] - " << "CLIENT " << communicationSocket << ":  " << buffer <<
                        "\n";
                }
                // If not, reject the message and notify them.
                else
                {
                    // notify user.
                }

                // Clear the buffer
                ZeroMemory(buffer, messageSize);
            }
        }
    }
}

void Server::ReadCommand()
{
    // Convert the command to a string
    std::string command(buffer);

    // Convert to lowercase
    std::transform(command.begin(), command.end(), command.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Convert to collection of strings (required for commands with multiple parameters)
    std::vector<std::string> commandArguments;
    std::stringstream commandStream(command);
    std::string arg;
    while (std::getline(commandStream, arg, ' '))
    {
        commandArguments.push_back(arg);
    }
    if (commandArguments[0] == "$register")
    {
        clients.at(*activeSocket)->isRegistered = true;

        //commands.Register();
    }
    if (commandArguments[0] == "$exit")
    {
        //commands.Exit();
    }
    else if (commandArguments[0] == "$getlist")
    {
        //commands.GetList();
    }
    else if (commandArguments[0] == "$getlog")
    {
        //commands.GetLog();
    }
    // Not a valid command, reject and notify client
    else
    {
    }
}

void Server::OnClientConnect(SOCKET client)
{
    std::cout << client;
    clients.insert({client, new ClientProfile()});
    std::cout << clients.at(client);
    //clients.insert(std::make_pair(client, new ClientProfile(client)));
    std::cout << "[" << "00:00:00" << "] - " << "SERVER:      " << "CLIENT " << "client" << " CONNECTED\n";
}

void Server::OnClientDisconnect()
{

}

void Server::LogAction(char* message) 
{
    //std::cout << "[00:00:00] - " << "SERVER:      " << "CLIENT " << client << " CONNECTED\n";
}