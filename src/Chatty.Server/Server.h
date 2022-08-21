#pragma once
#include "TcpService.h"

class Server : public TcpService
{
public:
	Server(const char* ipAddress, int port) :
		TcpService(ipAddress, port), serverLoop(true){ }

	bool InitServer();

	void Run() override;

	void ParseCommand();

	void OnClientConnect();

	void OnClientDisconnect();

	void LogAction(int mode, char* message);

private:
	FD_SET						masterSet;		// The set of ALL file descriptors
	FD_SET						readySet;		// The set of READY file descriptors
	bool						serverLoop;		// Used to terminate the main server loop
	std::map<SOCKET, char*>		clients;		// A map of clients, contains SOCKET # and client Username
};
