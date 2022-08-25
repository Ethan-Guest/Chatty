#pragma once
#include "TcpService.h"
#include "ClientProfile.h"
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include <fstream>

class Server : public TcpService
{
public:
    Server(const char* ipAddress, uint16_t port, int maxClients) :
        TcpService(ipAddress, port), clientCapacity(maxClients), serverMode(true)
    {
    }

    bool InitServer(); // Initialize the server

    void Run() override; // The main server loop

    void ReadCommand(); // Read commands and process them

    void OnClientConnect(SOCKET client); // Handle a client connection

    void OnClientDisconnect(SOCKET client); // Handle a client disconnection

    void LogAction(const std::list<std::string>& myArguments); // Print to console and save to log

    void BroadcastMessage(); // Broadcast TCP information to clients

    void GenerateLogFile(); // Generate a log file for the server session

private:
    int clientCapacity; // The max number of clients allowed in the server
    bool serverMode; // Used to terminate the main server loop
    std::string serverLogFileName; // The name of the log file generated for this session
    std::map<SOCKET, ClientProfile*> clients;
    // A collection of clients, contains SOCKET #, username, registration status
    FD_SET masterSet; // The set of ALL file descriptors
    FD_SET readySet; // The set of READY file descriptors
    SOCKET* activeSocket; // The socket that is currently being used
};