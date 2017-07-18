#include "BroadcastSocket.h"

BroadcastSocket::BroadcastSocket(int port_to_send)
{
    memset(&broadcast_addr_, 0, sizeof(broadcast_addr_));
    broadcast_addr_.sin_family = AF_INET;   //ipv4
    broadcast_addr_.sin_port = 
        htons(port_to_send != -1 ? (unsigned int)port_to_send : PORT); //defined in Defines.h
    
    //wanted to change it to inet_pton, but it doesn't work for some reason
    //BROADCAST_IP defined in Defines.h
    broadcast_addr_.sin_addr.s_addr = inet_addr(BROADCAST_IP);
}

BroadcastSocket::~BroadcastSocket(){}

bool BroadcastSocket::Initialize()
{
    if(AbstractSocket::Initialize(UDP) != true)
    {
        return false;
    }

    char broadcast_enabled = 1;
    if(setsockopt(socket_,
                  SOL_SOCKET,           //socket lvl option
                  SO_BROADCAST,         //broadcast option
                  &broadcast_enabled,
                  sizeof(broadcast_enabled)) != 0)
    {
        return false;
    }

    return true;
}

int BroadcastSocket::Send(const void* buffer, int size)
{
    return sendto(socket_,
                  (const char*)buffer,
                  size,
                  0,
                  (SOCKADDR*)&broadcast_addr_,
                  (int)sizeof(broadcast_addr_));
}

void BroadcastSocket::SetBroadcastPort(int port)
{
    broadcast_addr_.sin_port = htons(port);
}

int BroadcastSocket::SendTo(const void *buffer, int size, const char* ip)
{
    sockaddr_in send_to_addr = broadcast_addr_;
    send_to_addr.sin_addr.s_addr = inet_addr(ip);
    return sendto(socket_,
                  (const char*)buffer,
                  size,
                  0,
                  (SOCKADDR*)&send_to_addr,
                  (int)sizeof(send_to_addr));
}
