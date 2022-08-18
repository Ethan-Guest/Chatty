#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>



int main()
{
    // Initialize winsock
	WSADATA wsadata;
	if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
	{
		// Winsock startup error
	}
	
	std:: cout << "[" << "00:00:00" << "]" << " STARTUP SUCCESSFUL\n";

	// Create a listening socket
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		// Socket creation error
	}

	std::cout << "[" << "00:00:00" << "]" << " LISTENING SOCKET CREATED\n";


	// Initialize socket address
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddr.sin_port = htons(31337);


	// Bind
	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		// Binding error
	}

	std::cout << "[" << "00:00:00" << "]" << " LISTENING SOCKET BINDING SUCCESSFUL\n";


	// Listen
	if (listen(listenSocket, 1) != 0)
	{
		// Listening error
	}

	std::cout << "[" << "00:00:00" << "]" << " SERVER IS LISTENING\n";


	// Accept
	SOCKET communicationSocket = accept(listenSocket, NULL, NULL);
	if (communicationSocket == INVALID_SOCKET)
	{
		// Socket creation error
	}
	std::cout << "[" << "00:00:00" << "]" << " CLIENT ACCEPTED\n";



    ExitProcess(EXIT_SUCCESS);
}
