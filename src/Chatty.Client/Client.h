#pragma once
#include "TcpService.h"

class Client : public TcpService
{
public:
	Client(const char* ipAddress, uint16_t port) :
		TcpService(ipAddress, port), clientMode(true)
	{
	}

	bool InitClient(); // Initialize the client

	void Run() override; // The main client loop


private:
	bool clientMode;
	std::string userName; // Cleint username
};
