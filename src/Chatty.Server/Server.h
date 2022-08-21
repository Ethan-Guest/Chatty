#pragma once
#include "TcpService.h"
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include "ServerCommands.h"
#include "ClientProfile.h"

class Server : public TcpService
{
public:
	Server(const char* ipAddress, uint16_t port) :
		TcpService(ipAddress, port), serverMode(true)
	{
	}

	bool InitServer();

	void Run() override;

	void ReadCommand();

	void OnClientConnect(SOCKET client);

	void OnClientDisconnect();

	void LogAction(char* message);
				


private:
	FD_SET masterSet; // The set of ALL file descriptors
	FD_SET readySet; // The set of READY file descriptors
	bool serverMode; // Used to terminate the main server loop
	//ServerCommands						commands;		// Server commands
	std::map<SOCKET, ClientProfile*> clients; // A map of clients, contains SOCKET # and client Username
	SOCKET* activeSocket; // The socket that is currently being read
};
