#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#include "Server.h"
#include <Helper.h>


bool Server::InitServer()
{
    // Create log file
    GenerateLogFile();

    // Initialize winsock
    if (!InitWinsock(true))
    {
        return false;
    }

    // TCP SETUP
    // 
    // Bind the TCP socket to the ip address
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

    // UDP BROADCAST SETUP
    // 
    // CREATE UDP SOCKET
    broadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // UDP BROADCAST OUTPUT STRUCTURE
    broadcastAddr.sin_family = AF_INET; // AF_INET = IPv4 addresses
    broadcastAddr.sin_port = htons(31337); // Little to big endian conversion
    inet_pton(AF_INET, "127.0.0.1", &broadcastAddr.sin_addr); // Convert from string to byte array

    int optVal = 1;
    setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&optVal, sizeof(optVal));

    // Bind the UDP socket to the outputAddr
    if (bind(broadcastSocket, (SOCKADDR*)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR)
    {
        // Binding error
        return false;
    }
    return true;
}

void Server::GenerateLogFile()
{
    // Create the log file with unique name
    time_t curr_time;
    tm* curr_tm;
    char time_string[100];
    time(&curr_time);
    curr_tm = localtime(&curr_time);

    // Get the year:month:day
    strftime(time_string, 50, "%F", curr_tm);
    std::string yymmdd(time_string);
    serverLogFileName = "server_log-[" + yymmdd + "-";


    // Get the hour:minute:second
    strftime(time_string, 50, "%T", curr_tm);
    std::string hhmmss(time_string);
    serverLogFileName += hhmmss + "].txt";
    std::replace(serverLogFileName.begin(), serverLogFileName.end(), ':', '-'); // replace all ':' with '-'

    // Open the file once to create it
    std::ofstream out(serverLogFileName, std::ios_base::app);
    out.close();
}

void Server::Run()
{
    // Create new thread for broadcasting TCP information
    std::thread broadcastTCPMessage(&Server::BroadcastMessage, this);

    while (serverMode)
    {
        while (run.load())
        {
            // Copy original set
            readySet = masterSet;

            // 1 second timeout
            TIMEVAL timeout;
            timeout.tv_sec = 1;

            // Select incoming sockets
            int readySockets = select(0, &readySet, nullptr, nullptr, &timeout);

            // Loop through ready sockets
            for (int i = 0; i < readySockets; i++)
            {
                SOCKET communicationSocket = readySet.fd_array[i];
                activeSocket = &communicationSocket;


                // Check if listening socket is SET and accept new connection
                if (FD_ISSET(connectionSocket, &readySet))
                {
                    // Accept a new connection
                    SOCKET client = accept(connectionSocket, nullptr, nullptr);


                    OnClientConnect(client);
                }
                else
                {
                    // Accept new message

                    // Receive the size of the message
                    int bytes = TcpRecieveMessage(communicationSocket, (char*)&messageSize, 1);

                    // Set the buffer size
                    buffer = new char[messageSize];

                    // If receive failed
                    if (bytes <= 0)
                    {
                        // Close connection
                        OnClientDisconnect(communicationSocket);

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
                        TcpSendFullMessage(*activeSocket,
                                           "ERROR: Please use $register <username> before sending messages.");
                    }

                    // Clear the buffer
                    ZeroMemory(buffer, messageSize);
                }
            }
        }
    }
    // Close the thread
    run.store(false);
    broadcastTCPMessage.join();
}

void Server::BroadcastMessage()
{
    // Message to broadcast
    std::string broadcastMessage = "127.0.0.1 31337";

    //// Create the broadcast socket address
    //sockaddr_in broadcastAddress;
    //broadcastAddress.sin_family = AF_INET;
    //broadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    //broadcastAddress.sin_port = htons(31337);

    // Broadcast the message once per second
    while (run.load())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "BROADCASTING ON: " + broadcastMessage + "\n";
        int result =
            sendto(broadcastSocket, broadcastMessage.c_str(), broadcastMessage.size() + 1, 0,
                   (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    }
}

void Server::ReadCommand()
{
    auto args = Helper::CharPtrToVector(buffer, ' ', true);

    if (args[0] == "$register")
    {
        if (clients.at(*activeSocket)->isRegistered == true)
        {
            TcpSendFullMessage(*activeSocket, "ERROR: You are already registered.");
            return;
        }
        if (args.size() <= 1)
        {
            // Notify the user that no name is provided
            std::string activeSocketStr = SocketToString(*activeSocket);
            LogAction({"ERROR: CLIENT", activeSocketStr, "USED $REGISTER BUT DID NOT PROVIDE A NAME."});
            TcpSendFullMessage(*activeSocket, "ERROR: Please provide a username with $register <username>");
            return;
        }
        // Successful Registration
        if (clients.size() <= clientCapacity)
        {
            clients.at(*activeSocket)->isRegistered = true;
            clients.at(*activeSocket)->userName = args[1];
            LogAction({"SERVER: CLIENT", SocketToString(*activeSocket), "REGISTERED AS", args[1]});
            TcpSendFullMessage(*activeSocket, "SV_SUCCESS");
        }
        else if (clients.size() > clientCapacity)
        {
            // No space, notify client
            LogAction({"ERROR: CLIENT CAPACITY FULL"});
            TcpSendFullMessage(*activeSocket, "SV_FULL");

            // Close connection
            OnClientDisconnect(*activeSocket);
        }
    }

    else if (args[0] == "$getlist")
    {
        // If the client is registered
        if (clients.at(*activeSocket)->isRegistered)
        {
            std::string message;
            if (clients.size() <= 1)
            {
                message = "You are the only client connected.";

                TcpSendFullMessage(*activeSocket, message);
            }
            else
            {
                // Construct a message out of the clients collection
                for (const auto& x : clients)
                {
                    // The clients socket
                    std::string socketId = SocketToString(x.first);

                    // The clients username
                    std::string userName;
                    if (x.second->userName == "")
                    {
                        userName = "<username>";
                    }
                    else
                    {
                        userName = x.second->userName;
                    }

                    // The clients registration status
                    std::string status;
                    if (x.second->isRegistered)
                    {
                        status = "registered";
                    }
                    else
                    {
                        status = "unregistered";
                    }

                    // concatenate the above strings to construct a message
                    message += socketId + " : " + userName + " : " + status + ",\n";
                }

                // Remove the last comma and send the message
                message.erase(message.length() - 2, 1);
                TcpSendFullMessage(*activeSocket, message);
            }
        }
        // If the user is not registered
        else
        {
            LogAction({"ERROR: CLIENT", SocketToString(*activeSocket), "USED $GETLIST BUT IS NOT REGISTERED."});
            TcpSendFullMessage(*activeSocket, "Please register with $register before using $getlist.");
        }
    }
    else if (args[0] == "$help")
    {
        TcpSendFullMessage(*activeSocket,
                           "Server commands:\n$register <username> - Registers a client.\n$getlist - Get a list of all other connected clients.\n$getlog - Get a log of all server actions and messages.\n$exit - Close the connection to the server.\n");
    }

    if (args[0] == "$exit")
    {
        // Close connection
        OnClientDisconnect(*activeSocket);
    }
    else if (args[0] == "$getlog")
    {
        // Check if the client is registered
        if (clients.at(*activeSocket)->isRegistered)
        {
            TcpSendFullMessage(*activeSocket, "\nCURRENT SERVER LOG:\n");

            // Create ifstream
            std::ifstream serverLog(serverLogFileName);

            // Get each line of the file
            std::string currentLine;
            while (std::getline(serverLog, currentLine))
            {
                // Send the current line to the client
                std::istringstream stringStream(currentLine);
                TcpSendFullMessage(*activeSocket, currentLine);
            }
            LogAction({"CLIENT", SocketToString(*activeSocket), "USED $GETLOG"});
        }
        else
        {
            LogAction({"ERROR: CLIENT", SocketToString(*activeSocket), "USED $GETLOG BUT IS NOT REGISTERED."});
            TcpSendFullMessage(*activeSocket, "Please register with $register before using $getlog.");
        }
    }
    // Not a valid command, reject and notify client
    else
    {
    }
}



void Server::OnClientConnect(SOCKET client)
{
    // add the new connection to the list of connected clients
    FD_SET(client, &masterSet);

    // Add the un-registered client to the collection of clients
    clients.insert({client, new ClientProfile()});

    // Write to console + log
    std::string clientStr = SocketToString(client);
    LogAction({"CLIENT", clientStr, "CONNECTED"});
}

void Server::OnClientDisconnect(SOCKET client)
{
    // Remove the client from list of clients
    clients.erase(client);

    // Log client disconnect
    std::string clientStr = SocketToString(client);
    LogAction({"CLIENT", clientStr, "DISCONNECTED"});

    // Close the connection
    shutdown(client, SD_BOTH);
    closesocket(client);
    FD_CLR(client, &masterSet);
}

void Server::LogAction(const std::list<std::string>& myArguments)
{
    // Print the time (ew...)
    time_t curr_time;
    tm* curr_tm;
    char time_string[100];
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    strftime(time_string, 50, "%T", curr_tm);
    std::string hhmmss(time_string);
    std::string str;
    str += "[" + hhmmss + "] - ";

    // Concatenate the arguments
    for (auto elem : myArguments)
    {
        str += elem + " ";
    }
    str += "\n";

    // Print the string to console
    std::cout << str;

    // Log the string to server_log.txt
    std::ofstream out(serverLogFileName, std::ios_base::app);
    out << str;
    out.close();
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
