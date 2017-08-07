#include "TCPSocket.h"

#include <WS2tcpip.h>

TCPSocket::TCPSocket() {}

TCPSocket::TCPSocket(const SOCKET &sock) : AbstractSocket(sock){}

TCPSocket::~TCPSocket() {}

bool TCPSocket::Initialize()
{
    if(!AbstractSocket::Initialize(TCP))
    {
        Close();
        return false;
    }

    return true;
}

bool TCPSocket::Connect(const char* ip, unsigned int port)
{
    if(socket_ == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in sock_addr;

    //set server port and IP
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port ? port : FILE_PORT); //Host TO Network Short
    inet_pton(sock_addr.sin_family, ip, &(sock_addr.sin_addr));

    //trying to connect 
    for(int i = 0; i < 4; ++i)
    {
        if(i == 3)
        {
            return false;
        }
        else if(connect(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
        {
            Sleep(1000);
        }
        else
        {
            break;
        }
    }

    return true;
}

bool TCPSocket::SetListen(unsigned int port, int num_of_connections)
{
    sockaddr_in sock_addr = { AF_INET,
                              htons(port ? port : FILE_PORT),
                              INADDR_ANY };

    if(socket_ == INVALID_SOCKET)
    {
        return false;
    }

    if(bind(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
    {
        Close();
        return false;
    }

    if(listen(socket_,
              num_of_connections ? num_of_connections : SOMAXCONN) != 0)
    {
        Close();
        return false;
    }

    return true;
}

int TCPSocket::Send(const char* buffer, int size)const
{
    return send(socket_, buffer, size, 0);
}

int TCPSocket::Recv(char* buffer, int size)const
{
    return recv(socket_, buffer, size, 0);
}

bool TCPSocket::Accept(TCPSocket &out_socket)const
{
    SOCKET temp_sock = accept(socket_, NULL, NULL);
    if(temp_sock == INVALID_SOCKET)
    {
        return false;
    }

    out_socket.SetSocket(temp_sock);
    return true;
}

bool TCPSocket::IsConnectionCame(unsigned int msec_timeout)const
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);
    TIMEVAL timeout = { msec_timeout / 1000, (msec_timeout % 1000) * 1000 };

    int status = select(0, &sockets, NULL, NULL, &timeout);

    if(status == -1 || status == 0)
        return false;

    return true;
}