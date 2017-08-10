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

bool TCPSocket::Connect(const char* ip, unsigned int port, unsigned int timeout_sec)
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

    int timeout = (timeout_sec ? timeout_sec : 0);
    //trying to connect 
    for(int i = 0; i <= timeout * 4; ++i)
    {
        //if connection succeed
        if(connect(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
            break;
        //if timeout occurs
        else if(i == timeout)
            return false;
        //wait a second
        Sleep(250);
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

bool TCPSocket::ConnectionCame(unsigned int msec_timeout)const
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);
    TIMEVAL timeout = { msec_timeout / 1000, (msec_timeout % 1000) * 1000 };

    int status = select(0,          //not used
                        &sockets,   //check these sockets to read (and accept)
                        NULL,       //check these sockets to write
                        NULL,       //check these sockets exceptions
                        &timeout);  //timeout
    //if select doesn't fail and return value isn't 0
    if(status == -1 || status == 0)
        return false;

    return true;
}

bool TCPSocket::TryAccept(TCPSocket &out_socket, unsigned int msec_timeout)const
{
    if(!ConnectionCame(msec_timeout))
        return false;

    if(!Accept(out_socket))
        return false;

    return true;
}