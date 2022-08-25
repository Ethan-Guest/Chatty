#define _CRT_SECURE_NO_WARNINGS                 // turns of deprecated warnings
#define _WINSOCK_DEPRECATED_NO_WARNINGS         // turns of deprecated warnings for winsock

#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <ctime>
#include <thread>
#include <atomic>

#pragma comment (lib, "ws2_32.lib")

class TcpService {

public:
	// Service constructors
	TcpService() :
		run(true)
	{
	}
	TcpService(const char* _ipAddress, int _port) :
		ipAddress(_ipAddress), port(_port), run(true)
	{
	}
protected:

	// Initialize the service
	bool InitWinsock(bool serverMode);

	// Run the service
	virtual void Run();

	// Client waits for UDP broadcast
	bool ClientBroadcastReceive();

	int TcpSendMessage(SOCKET socket, const char* data, uint16_t length);

	void TcpSendFullMessage(SOCKET s, std::string message); // Send the client a message in one method

	int TcpRecieveMessage(SOCKET socket, char* buf, int length);

	SOCKET connectionSocket; // The listening / connection socket
	SOCKET broadcastSocket; // The socket used for sending / receiving broadcast information from server

	SOCKADDR_IN socketAddress; // The socket address
	SOCKADDR_IN broadcastAddr;

	uint16_t messageSize;
	char* buffer;
	std::atomic<bool> run; // Synchronizing bool

	const char* ipAddress;

	uint16_t port;


private:
	std::string broadcastReceiveMessage;

};