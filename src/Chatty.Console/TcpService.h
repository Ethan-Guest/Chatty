#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

class TcpService {

public:

	// ctor

	// Initialize the service
	virtual bool Init();

	// Run the service
	virtual int Run();

protected:

	void TcpSendMessage();
	void TcpRecieveMessage();

private:
	const char* ipAddress;
	int port;
	SOCKET socket;
};