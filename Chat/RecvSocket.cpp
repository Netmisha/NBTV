#include "RecvSocket.h"

RecvSocket::RecvSocket() : buffer_(NULL)
{
    //'()' for zero`ing alocated memory
    buffer_ = new char[BUFFER_SIZE]();
}

RecvSocket::~RecvSocket()
{
    delete[] buffer_;
}

bool RecvSocket::Initialize(int port)
{
    if(!AbstractSocket::Initialize(UDP))
    {
        return false;
    }

    char broadcast_enabled = 1;
    if(setsockopt(socket_,
                  SOL_SOCKET,       //socket lvl option
                  SO_BROADCAST,     //broadcast option
                  &broadcast_enabled,
                  sizeof(broadcast_enabled) != 0))
    {
        return false;
    }

    sockaddr_in port_bind;
    memset(&port_bind, 0, sizeof(port_bind));
    port_bind.sin_family = AF_INET;
    port_bind.sin_port = htons(port != -1 ? (unsigned int)port : PORT);
    port_bind.sin_addr.s_addr = INADDR_ANY;
    
    return bind(socket_, (SOCKADDR*)&port_bind, (int)sizeof(port_bind)) == 0;
}

int RecvSocket::Recv(RecvStruct* out_result)
{
    memset(buffer_, 0, BUFFER_SIZE);
    sockaddr_in recv_info;
    int recv_info_size = sizeof(recv_info);
    
    int recv_size = recvfrom(socket_,
                             buffer_,
                             BUFFER_SIZE,
                             0,
                             (SOCKADDR*)&recv_info,
                             &recv_info_size);
    if(recv_size == SOCKET_ERROR)
    {
        return -1;
    }

    inet_ntop(AF_INET, &(recv_info.sin_addr), &out_result->ip_[0], IP_SIZE);
    out_result->packet_ = new char[recv_size]();
    memcpy(out_result->packet_, buffer_, recv_size);

    return recv_size;
}