#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace Helper
{
    // Converts a char* to a vector of words
    static std::vector<std::string> CharPtrToVector(char* string, char delimiter, bool lowercase)
    {
        // Convert the command to a string
        std::string command(string);

        // Convert to lowercase
        if (lowercase)
        {
            std::transform(command.begin(), command.end(), command.begin(),
                           [](unsigned char c) { return std::tolower(c); });
        }

        // Convert to collection of strings (required for commands with multiple parameters)
        std::vector<std::string> commandArguments;
        std::stringstream commandStream(command);
        std::string arg;
        while (std::getline(commandStream, arg, delimiter))
        {
            commandArguments.push_back(arg);
        }
        return commandArguments;
    }
}
