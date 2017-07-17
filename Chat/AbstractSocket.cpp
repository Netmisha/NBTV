#include "AbstractSocket.h"

AbstractSocket::AbstractSocket() : socket_(INVALID_SOCKET)
{}

AbstractSocket::~AbstractSocket(){}

int AbstractSocket::Initialize(int type)
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

    return (socket_ == INVALID_SOCKET ? INVALID_SOCKET : 0);
}

int AbstractSocket::Close()
{
    int error_check = shutdown(socket_, SD_BOTH);
    if(error_check == 0)
        error_check = closesocket(socket_);
    return error_check;
}