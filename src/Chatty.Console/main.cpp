#include <iostream>
#include "Client.h"
#include "Server.h"

void StartServer();
void StartClient();

int main()
{
    bool exitProgram = false;
    int menuChoice = 0;
    while (!exitProgram)
    {
        std::cout << "Welcome to chatty. Please select an option:\n";
        std::cout << "1) Server\n2) Client\n3) Exit\n";

        while(!(std::cin >> menuChoice)){
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid option. Try again:\n";
        }
        switch(menuChoice) {
        case 1:
            StartServer();
            break;
        case 2:
            StartClient();
            break;
        case 3:
            exitProgram = true;
            break;
        default:
            break;
        }
    }
}

void StartServer()
{
    auto ipAddress = "127.0.0.1";
    uint16_t port = 31337;
    int maxClients = 3;
    //std::cout << "Server Configuration:\n";
    //std::cout << "IP: ";
    //std::cin >> ipAddress;
    //std::cout << "PORT: ";
    //std::cin >> port;
    //std::cout << "MAX ALLOWED CLIENTS: ";
    //std::cin >> maxClients;

    Server server(ipAddress, port, maxClients);
    if (server.InitServer())
    {
        server.Run();
    }
}

void StartClient()
{
    Client client;
    if (client.InitClient())
    {
        client.Run();
    }
}


bool UDPClient()
{
    //// Initialize winsock
    //WSADATA wsadata;
    //if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0)
    //{
    //    // TODO: Winsock startup error
    //    return false;
    //}
    //std::cout << "[" << "00:00:00" << "] - WINSOCK INITIALIZED\n";

    //// Create a UDP socket
    //SOCKET in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //// Create a client sockaddr
    //sockaddr_in clientAddr;
    //clientAddr.sin_addr.S_un.S_addr = ADDR_ANY;
    //clientAddr.sin_family = AF_INET;
    //clientAddr.sin_port = htons(31337);

    //// Bind the socket to the IP and port
    //if (bind(listening, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
    //{
    //    std::cout << "ERROR: CANT BIND SOCKET." << WSAGetLastError() + "\n";
    //    return false;
    //}
    return true;
}