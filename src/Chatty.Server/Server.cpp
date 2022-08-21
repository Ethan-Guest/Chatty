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
    LogAction({"SERVER:", "LISTENING SOCKET BINDING SUCCESSFUL"});

    // Start listening on the server socket
    if (listen(connectionSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        // Listening error
        return false;
    }

    LogAction({"SERVER:", "LISTENING MODE ENABLED"});

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
                    // Log disconnect

                    // Drop the client
                    OnClientDisconnect(communicationSocket);
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
                    std::string activeSocketStr = SocketToString(*activeSocket);
                    LogAction({"ERROR: FAILED TO READ INCOMING MESSAGE FROM CLIENT", activeSocketStr});
                }
                if (buffer[0] == '$')
                {
                    ReadCommand();
                }
                // If the user is registered, read the message. 
                else if (clients.at(communicationSocket)->isRegistered)
                {
                    LogAction({clients.at(*activeSocket)->userName, ":", buffer});
                }
                // If not, reject the message and notify them.
                else if (!clients.at(communicationSocket)->isRegistered)
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
        if (commandArguments.size() <= 1)
        {
            // Notify the user that no name is provided
            std::string activeSocketStr = SocketToString(*activeSocket);
            LogAction({"ERROR: CLIENT", activeSocketStr, "USED $REGISTER BUT DID NOT PROVIDE A NAME."});
            return;
        }
        // If there is space for the client
        if (clients.size() <= clientCapacity)
        {
            clients.at(*activeSocket)->isRegistered = true;
            clients.at(*activeSocket)->userName = commandArguments[1];
            std::string activeSocketStr = SocketToString(*activeSocket);
            LogAction({"SERVER: CLIENT", activeSocketStr, "REGISTERED AS", commandArguments[1]});
        }
        else
        {
            LogAction({"ERROR: CLIENT CAPACITY FULL"});

            // No space, notify client
        }


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
    // Add the un-registered client to the collection of clients
    clients.insert({client, new ClientProfile()});

    std::string clientStr = SocketToString(client);
    LogAction({"CLIENT", clientStr, "CONNECTED"});
}

void Server::OnClientDisconnect(SOCKET client)
{
    clients.erase(client);

    std::string clientStr = SocketToString(client);
    LogAction({"CLIENT", clientStr, "DISCONNECTED"});
}

void Server::LogAction(const std::list<std::string>& myArguments)
{
    std::string str;

    str += "[00:00:00] - ";
    for (auto elem : myArguments)
    {
        str += elem + " ";
    }
    str += "\n";

    std::cout << str;
}

std::string Server::ObjToString(void* param)
{
    std::stringstream stringstream;
    stringstream << param;
    return stringstream.str();

}

std::string Server::SocketToString(SOCKET s)
{
    std::stringstream stringstream;
    stringstream << s;
    return stringstream.str();

}
