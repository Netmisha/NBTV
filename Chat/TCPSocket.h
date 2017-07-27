#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "AbstractSocket.h"
#include "Defines.h"

class TCPSocket : public AbstractSocket
{
public:
    TCPSocket();
    TCPSocket(const SOCKET &sock);
    ~TCPSocket();

    bool Initialize();
    bool Connect(const char* ip, unsigned int port = 0);
    bool SetListen(unsigned int port = 0, int num_of_connections = 0);
    TCPSocket Accept()const;
    
    int Send(const char* buffer, int size)const;
    int Recv(char* buffer, int size)const;
};

#endif // !TCP_SOCKET_H