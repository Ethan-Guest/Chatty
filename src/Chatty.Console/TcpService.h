#define _CRT_SECURE_NO_WARNINGS                 // turns of deprecated warnings
#define _WINSOCK_DEPRECATED_NO_WARNINGS         // turns of deprecated warnings for winsock

#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <ctime>
#include <thread>
#include <atomic>

#pragma comment (lib, "ws2_32.lib")

class TcpService
{
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

    // Send a message
    int TcpSendMessage(SOCKET socket, const char* data, uint16_t length);

    // Send a message (length and message both handled) 
    void TcpSendFullMessage(SOCKET s, std::string message);

    // Receive a message
    int TcpRecieveMessage(SOCKET socket, char* buf, int length);

    SOCKET connectionSocket; // A TCP listening / connection socket
    SOCKET broadcastSocket; // A UDP socket used for sending / receiving broadcast information from server

    SOCKADDR_IN socketAddress; // TCP socket address structure
    SOCKADDR_IN broadcastAddr; // UDP socket address structure

    char* buffer; // The buffer for sending / receiving data
    const char* ipAddress; // The IP address used to connect

    uint16_t messageSize; // The size of the incoming / outgoing message
    uint16_t port; // The port used to connect

    std::atomic<bool> run; // Synchronizing bool for multi-threading


private:
    std::string broadcastReceiveMessage;
};
