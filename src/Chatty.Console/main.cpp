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

        while (!(std::cin >> menuChoice))
        {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid option. Try again:\n";
        }
        switch (menuChoice)
        {
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
    int maxClients;
    //std::cout << "Server Configuration:\n";
    //std::cout << "IP: ";
    //std::cin >> ipAddress;
    //std::cout << "PORT: ";
    //std::cin >> port;
    std::cout << "MAX ALLOWED CLIENTS: ";
    std::cin >> maxClients;

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
