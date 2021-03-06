#include "BroadcastSocket.h"

BroadcastSocket::BroadcastSocket(unsigned int port_to_send)
{
    memset(&broadcast_addr_, 0, sizeof(broadcast_addr_));
    broadcast_addr_.sin_family = AF_INET;   //ipv4
    broadcast_addr_.sin_port = 
        htons(port_to_send ? port_to_send : PORT); //defined in Defines.h
    
    //BROADCAST_IP defined in Defines.h
    inet_pton(broadcast_addr_.sin_family, BROADCAST_IP, &(broadcast_addr_.sin_addr));
}

BroadcastSocket::~BroadcastSocket(){}

bool BroadcastSocket::Initialize(unsigned int port_to_send)
{
    if(!AbstractSocket::Initialize(UDP))
    {
        Close();
        return false;
    }

    char broadcast_enabled = 1;
    if(setsockopt(socket_,
                  SOL_SOCKET,           //socket lvl option
                  SO_BROADCAST,         //broadcast option
                  &broadcast_enabled,
                  sizeof(broadcast_enabled)) != 0)
    {
        Close();
        return false;
    }

    if(port_to_send)
        SetBroadcastPort(port_to_send);

    return true;
}

int BroadcastSocket::Send(const void* buffer, int size)const
{
    send_mutex_.Lock();
    int return_val = sendto(socket_,
                            (const char*)buffer,
                            size,
                            0,
                            (SOCKADDR*)&broadcast_addr_,
                            (int)sizeof(broadcast_addr_));
    send_mutex_.Unlock();
    return return_val;
}

void BroadcastSocket::SetBroadcastPort(unsigned int port)
{
    broadcast_addr_.sin_port = htons(port);
}

int BroadcastSocket::SendTo(const void *buffer, int size, const char* ip)const
{
    //copy broadcast addr
    sockaddr_in send_to_addr = broadcast_addr_;
    //change ip to recieved one
    inet_pton(send_to_addr.sin_family, ip, &(send_to_addr.sin_addr));

    send_mutex_.Lock();
    int return_val = sendto(socket_,
                            (const char*)buffer,
                            size,
                            0,
                            (SOCKADDR*)&send_to_addr,
                            (int)sizeof(send_to_addr));
    send_mutex_.Unlock();

    return return_val;
}
