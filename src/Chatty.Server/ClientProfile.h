#pragma once
#include "TcpService.h"

class ClientProfile
{
public:
    // Profile constructor
    ClientProfile() :
        userName(""), isRegistered(false)
    {
    }

    std::string userName; // The username of the client
    bool isRegistered; // Whether or not the user is registered
};