#include <iostream>

void readInput(std::string input);

int main()
{
    // display initial startup messages
    std::cout << "[SERVER READY]!\n";
    std::cout << "To start the server use '--start'\n";
    std::cout << "To exit the server use '--exit'\n";

    // store user input
    std::string input;
    
    while(true)
    {
        
        std::cin >> input;
        
        if (input == "--start")
        {
            //start();
        }
        else if (input == "--exit")
        {
            return 0;
        }
        else
        {
            std::cout << "'" << input << "'" << " is not a recognized command.\n";
        }
    }


}


