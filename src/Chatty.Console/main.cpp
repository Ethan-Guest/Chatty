#include <iostream>

#include "../Chatty.Server/Server.h"

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
    // TODO: Prompt for address and port
    Server server("127.0.0.1", 31337);
    if (server.InitServer())
    {
        server.Run();
    }
}
void StartClient()
{
    
}