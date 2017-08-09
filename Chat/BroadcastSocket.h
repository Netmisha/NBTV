#ifndef BROADCAST_SOCKET_H
#define BROADCAST_SOCKET_H

#include "AbstractSocket.h"
#include "Defines.h"
#include "Mutex.h"

#include <WS2tcpip.h>

class DLL BroadcastSocket : public AbstractSocket
{
public:
    BroadcastSocket(unsigned int port_to_send = 0U);
    ~BroadcastSocket();

    //calls AbstractSocket::Initialize
    //and finishes initializetion
    bool Initialize(unsigned int port_to_send = 0U);

    //broadcasts message to the entire network
    //returns size of message send in bytes
    //or -1 in case of failure
    int Send(const void *buffer, int size)const;

    //sends message to specific ip
    //returns size of message send in bytes
    //or -1 in case of failure
    int SendTo(const void *buffer, int size, const char* ip)const;

    //sets socket to broadcast to different port
    void SetBroadcastPort(unsigned int port);

private:
    //broadcast address
    sockaddr_in broadcast_addr_;
    //mutex on send
    Mutex send_mutex_;
};

#endif // !BROADCAST_SOCKET_H