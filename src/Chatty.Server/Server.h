#pragma once
#include "TcpService.h"
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include "ServerCommands.h"
#include "ClientProfile.h"

class Server : public TcpService
{
public:
	Server(const char* ipAddress, uint16_t port, int maxClients) :
		TcpService(ipAddress, port), serverMode(true), clientCapacity(maxClients)
	{
	}

	bool InitServer(); // Initialize the server

	void Run() override; // The main server loop

	void ReadCommand(); // Read commands and process them

	void OnClientConnect(SOCKET client); // Handle a client connection

	void OnClientDisconnect(SOCKET client); // Handle a client disconnection

	void LogAction(const std::list<std::string>& myArguments); // Print to console and save to log

	//TODO: Move these to static helper library or DLL
	std::string ObjToString(void* param); // Helper function for converting an OBJECT of unknown type to a string

	std::string SocketToString(SOCKET s); // Helper function for converting an SOCKET to a string

private:
	FD_SET masterSet; // The set of ALL file descriptors
	FD_SET readySet; // The set of READY file descriptors
	bool serverMode; // Used to terminate the main server loop
	//ServerCommands						commands;		// Server commands
	std::map<SOCKET, ClientProfile*> clients; // A map of clients, contains SOCKET # and client Username
	SOCKET* activeSocket; // The socket that is currently being read
	int clientCapacity; // The max number of clients allowed in the server
};
