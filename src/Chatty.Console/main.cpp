#include <iostream>
#include "Client.h"
#include "Server.h"

void StartServer();
void StartClient();

int main()
{
    bool exitProgram = false;
    int menuChoice = 0;

    // Main console app loop
    while (!exitProgram)
    {
        std::cout << "Welcome to chatty. Please select an option:\n";
        std::cout << "1) Server\n2) Client\n3) Exit\n";

        while (!(std::cin >> menuChoice))
        {
            std::cin.clear();
            std::cout << "Invalid option. Try again:\n";
        }
        switch (menuChoice)
        {
        case 1:
            StartServer();
            continue;
        case 2:
            StartClient();
            continue;
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
    std::string ipAddress;
    uint16_t port;
    int maxClients;

    // Get user input
    std::cout << "Server Configuration:\n";
    std::cout << "IP: ";
    std::cin >> ipAddress;
    const char* ipAddressChar = ipAddress.c_str();
    std::cout << "PORT: ";
    std::cin >> port;
    std::cout << "MAX ALLOWED CLIENTS: ";
    std::cin >> maxClients;

    // Create the server with the information provided
    Server server(ipAddressChar, port, maxClients);

    // If the server starts successfully, run the main server loop
    if (server.InitServer())
    {
        server.Run();
    }
}

void StartClient()
{
    Client client;

    // If the client starts successfully, run the main client loop
    if (client.InitClient())
    {
        client.Run();
    }
}
