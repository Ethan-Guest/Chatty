#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <map>

#pragma comment (lib, "ws2_32.lib")

class TcpService {

public:

	// Service constructor
	TcpService(const char* _ipAddress, int _port) : 
		ipAddress(_ipAddress), port(_port) { }

protected:

	// Initialize the service
	bool InitWinsock();

	// Run the service
	virtual void Run();

	int TcpSendMessage(SOCKET socket, const char* data, int length);

	int TcpRecieveMessage(SOCKET socket, char* buf, int length);

	SOCKET				connectionSocket; // The listening / connection socket
	SOCKADDR_IN			socketAddress;	  // The socket address
	uint8_t				messageSize;
	char*				buffer;

private:

	const char*			ipAddress;
	int					port;
};