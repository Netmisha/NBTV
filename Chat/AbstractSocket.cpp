#include "AbstractSocket.h"

AbstractSocket::AbstractSocket() : socket_(INVALID_SOCKET) {}

AbstractSocket::AbstractSocket(const SOCKET &sock) : socket_(sock) {}

AbstractSocket::~AbstractSocket() 
{
    Close();
}

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

    return socket_ != INVALID_SOCKET;
}

bool AbstractSocket::Close()
{
    if(socket_ == INVALID_SOCKET)
    {
        //if socket is invalid there is no need to close it
        return true;
    }

    int error_check = shutdown(socket_, SD_BOTH);
    //you can't use shutdown on closed socket
    //so additionally check for error 'socket not connected'
    if((error_check == SOCKET_ERROR) && (GetLastError() != ERROR_SOCKET_NOT_CONNECTED))
        return false;

    error_check = closesocket(socket_);
    
    return error_check != SOCKET_ERROR;
}

void AbstractSocket::SetSocket(const SOCKET &socket)
{
    socket_ = socket;
}