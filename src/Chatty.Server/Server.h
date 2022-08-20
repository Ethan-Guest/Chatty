#pragma once
#include "TcpService.h"

class Server : public TcpService
{
public:
	Server(const char* ipAddress, int port) :
		TcpService(ipAddress, port)
	{
	}

	bool InitServer();

	void Run() override;

	void ParseCommand();

	void OnClientConnect();

	void OnClientDisconnect();

protected:
private:
	FD_SET masterSet;
	FD_SET readySet;
};
