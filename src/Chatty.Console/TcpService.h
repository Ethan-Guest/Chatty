#define _CRT_SECURE_NO_WARNINGS                 // turns of deprecated warnings
#define _WINSOCK_DEPRECATED_NO_WARNINGS         // turns of deprecated warnings for winsock

#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <ctime>

#pragma comment (lib, "ws2_32.lib")

class TcpService {

public:

	// Service constructor
	TcpService(const char* _ipAddress, int _port) : 
		ipAddress(_ipAddress), port(_port) { }

protected:

	// Initialize the service
	bool InitWinsock(bool serverMode);

	// Run the service
	virtual void Run();

	int TcpSendMessage(SOCKET socket, const char* data, uint16_t length);

	void TcpSendFullMessage(SOCKET s, std::string message); // Send the client a message in one method

	int TcpRecieveMessage(SOCKET socket, char* buf, int length);

	SOCKET				connectionSocket; // The listening / connection socket
	SOCKADDR_IN			socketAddress;	  // The socket address
	uint16_t messageSize;
	char*				buffer;

private:

	const char*			ipAddress;
	uint16_t port;
};