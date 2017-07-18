#include "AbstractSocket.h"

AbstractSocket::AbstractSocket() : socket_(INVALID_SOCKET) {}

AbstractSocket::~AbstractSocket() {}

bool AbstractSocket::Initialize(const SocketConnectionType &type)
{
    switch(type)
    {
        case TCP:
            socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            break;

        case UDP:
            socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            break;
    }

    return (socket_ == INVALID_SOCKET ? false : true);
}

bool AbstractSocket::Close()
{
    int error_check = shutdown(socket_, SD_BOTH);
    if(error_check == SOCKET_ERROR)
        return false;

    error_check = closesocket(socket_);
    
    return (error_check != SOCKET_ERROR ? true : false);
}