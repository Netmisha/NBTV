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
    bool Connect(const char* ip, unsigned int port = 0);
    //sets socket to listen
    //returns false in case of failure
    bool SetListen(unsigned int port = 0, int num_of_connections = 0);
    //accepts connection and returns TCPSocket as result
    bool Accept(TCPSocket &out_socket)const;
    //returns true if there is connection to accept
    //returns false if timeout expires and connection doesn't came
    bool IsConnectionCame(unsigned int msec_timeouts)const;
    
    //sends 'size' bytes from buffer
    //returns number of bytes send
    //-1 if send failed
    int Send(const char* buffer, int size)const;
    //recieves 'size' bytes to buffer
    //returns number of bytes recieved
    //-1 if send failed
    int Recv(char* buffer, int size)const;
};

#endif // !TCP_SOCKET_H