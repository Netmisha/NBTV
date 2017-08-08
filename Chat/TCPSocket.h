#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "AbstractSocket.h"
#include "Defines.h"

class DLL TCPSocket : public AbstractSocket
{
public:
    TCPSocket();
    TCPSocket(const SOCKET &sock);
    ~TCPSocket();

    //calls AbstractSocket initialize with TCP as parameter
    //returns false if fails
    bool Initialize();
    //connects socket to specific ip and port
    //returns false if fails
    //if timeout is not passed
    bool Connect(const char* ip, unsigned int port = 0, unsigned int timeout_sec = 0);
    //sets socket to listen
    //returns false in case of failure
    bool SetListen(unsigned int port = 0, int num_of_connections = 0);
    //accepts connection, returns socket as passed parameter
    //returns true if accept didn't fail
    //false otherwise
    bool Accept(TCPSocket &out_socket)const;
    //tries to accept connection
    //doesn't block thread
    //returns true if accepted, false otherwise
    bool TryAccept(TCPSocket &out_socket, unsigned int msec_timeout = 0)const;
    //sends 'size' bytes from buffer
    //returns number of bytes send
    //-1 if send failed
    int Send(const char* buffer, int size)const;
    //recieves 'size' bytes to buffer
    //returns number of bytes recieved
    //-1 if send failed
    int Recv(char* buffer, int size)const;

private:
    //returns true if there is connection to accept
    //returns false if timeout expires and connection doesn't came
    bool ConnectionCame(unsigned int msec_timeouts)const;
};

#endif // !TCP_SOCKET_H