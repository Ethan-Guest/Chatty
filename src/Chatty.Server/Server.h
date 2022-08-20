#pragma once
#include <string>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")

class Server
{
public:

    // Server class constructor
    Server(const char* ip_addr, int port) :
    ipAddress(ip_addr), port(port), messageSize(0), serverLoop(true) { }

    // Initialize winsock
    bool Init();

    // Run the server (main loop)
    void Run();

    // Send data
    int Send(SOCKET skSocket, const char* data, uint16_t length);

    // Receive data
    int Recv(SOCKET s, char* buf, int len);

    // Cleaup server
    void Cleanup();

private:

    // create the server listening socket
    SOCKET Create_Socket();

    // Select incoming connections
    SOCKET Net_Multiplexer();
    
    const char* ipAddress;
    int port;
    SOCKET listeningSocket;
    FD_SET masterSet;
    FD_SET readySet;

    uint8_t messageSize;
    char* buffer;

    TIMEVAL timeout;

    bool serverLoop;
};
