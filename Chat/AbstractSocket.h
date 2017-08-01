#ifndef ABSTRACT_SOCKET_H
#define ABSTRACT_SOCKET_H

#include <WinSock2.h>
#include <Windows.h>
#include "Defines.h"
#pragma comment(lib, "Ws2_32.lib")

enum SocketConnectionType
{
    // AF_INET address family
    // SOCK_STREAM type
    // IPPROTO_TCP protocol
    TCP = 0,

    // AF_INET address family
    // SOCK_DGRAM sock type
    // IPPROTO_UDP protocol
    UDP = 1
};

class AbstractSocket
{
public:
    AbstractSocket();
    AbstractSocket(const SOCKET &sock);
    virtual ~AbstractSocket() = 0;  //to make it abstract

    //initializes socket depending on passed SocketConnectionType
    //returns false if failed to initialize socket
    bool Initialize(const SocketConnectionType &type);

    //closes socket
    //returns false if failed to close socket
    bool Close();

protected:
    SOCKET socket_;
};

#endif // !ABSTRACT_SOCKET_H