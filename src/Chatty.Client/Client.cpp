#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>

uint16_t port = 31337;
const char* address = "127.0.0.1";

int tcp_send_whole(SOCKET s, const char* data, uint16_t length);
int tcp_recv_whole(SOCKET s, char* buf, int len);

// loop send func
int tcp_send_whole(SOCKET s, const char* data, uint16_t length)
{
	int result;
	int bytesSent = 0;

	while (bytesSent < length)
	{
		result = send(s, (const char*)data + bytesSent, length - bytesSent, 0);

		if (result <= 0)
			return result;

		bytesSent += result;
	}

	return bytesSent;
}

// loop recv func
int tcp_recv_whole(SOCKET s, char* buf, int len)
{
	int total = 0;

	do
	{
		int ret = recv(s, buf + total, len - total, 0);
		if (ret < 1)
			return ret;
		else
			total += ret;

	} while (total < len);

	return total;
}

int main()
{
	// Initialize winsock
	WSADATA wsadata;
	if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
	{
		// Winsock startup error
	}

	std::cout << "[" << "00:00:00" << "]" << " STARTUP SUCCESSFUL\n";


	// Create a communication socket
	SOCKET communicationSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (communicationSocket == INVALID_SOCKET)
	{
		// Socket creation error
	}

	std::cout << "[" << "00:00:00" << "]" << " COMMUNICATION SOCKET CREATED\n";


	// Initialize socket address
	SOCKADDR_IN clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.S_un.S_addr = inet_addr(address);
	if (clientAddr.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		// Address not in dotted-quadrant format
	}
	clientAddr.sin_port = htons(port);

	// Connect
	if (connect(communicationSocket, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) != 0)
	{
		// Connect error
	}

	std::cout << "[" << "00:00:00" << "]" << " COMMUNICATION SOCKET CONNECTED\n";


	//Communication
	uint8_t size = 30;
	char sendbuffer[30];
	memset(sendbuffer, 0, 30);
	strcpy(sendbuffer, "I'm a message from the client");

	int result = tcp_send_whole(communicationSocket, (char*)&size, 1);


	// close sockets
	shutdown(communicationSocket, SD_BOTH);
	closesocket(communicationSocket);


	ExitProcess(EXIT_SUCCESS);
}
