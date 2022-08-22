#pragma once
#include <iostream>
#include <WS2tcpip.h>

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

	int TcpSendMessage(SOCKET socket, const char* data, uint16_t length);

	int TcpRecieveMessage(SOCKET socket, char* buf, int length);

	SOCKET				connectionSocket; // The listening / connection socket
	SOCKADDR_IN			socketAddress;	  // The socket address
	uint16_t messageSize;
	char*				buffer;

private:

	const char*			ipAddress;
	uint16_t port;
};