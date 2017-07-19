#ifndef ABSTRACT_SOCKET_H
#define ABSTRACT_SOCKET_H

#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")
#define FILE_PORT 12000
enum SocketConnectionType
{
    TCP = 0,
    UDP = 1
};

class AbstractSocket
{
public:
    AbstractSocket();
    virtual ~AbstractSocket() = 0;  //to make it abstract

    //initializes socket as
    //SOCK_STREAM/tcp or DGRAM/udp
    //depending on 'type'
    bool Initialize(const SocketConnectionType &type);

    //closes socket
    bool Close();

protected:
    SOCKET socket_;
};

#endif // !ABSTRACT_SOCKET_H