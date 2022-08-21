#pragma once
#include "TcpService.h"

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

	void OnClientConnect();

	void OnClientDisconnect();

	void LogAction(char* message);



private:
	FD_SET						masterSet;		// The set of ALL file descriptors
	FD_SET						readySet;		// The set of READY file descriptors
	bool serverMode; // Used to terminate the main server loop
	std::map<SOCKET, ClientProfile> clients; // A map of clients, contains SOCKET # and client Username
	ServerCommands commands; // Server commands
};
