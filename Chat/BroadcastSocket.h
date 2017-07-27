#ifndef BROADCAST_SOCKET_H
#define BROADCAST_SOCKET_H

#include "AbstractSocket.h"
#include "Defines.h"

#include <WS2tcpip.h>

class BroadcastSocket : public AbstractSocket
{
public:
    BroadcastSocket(int port_to_send = -1);
    ~BroadcastSocket();

    //calls AbstractSocket::Initialize
    //and finishes initializetion
    bool Initialize();

    //broadcasts message to the network
    int Send(const void *buffer, int size)const;

    int SendTo(const void *buffer, int size, const char* ip)const;

    void SetBroadcastPort(int port);

private:
    //broadcast address
    sockaddr_in broadcast_addr_;
};

#endif // !BROADCAST_SOCKET_H