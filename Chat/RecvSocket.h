#ifndef RECVSOCKET_H
#define RECVSOCKET_H

#include "AbstractSocket.h"
#include "RecvStruct.h"
#include "Defines.h"

#include <WS2tcpip.h>

class RecvSocket : public AbstractSocket
{
public:
    RecvSocket();
    ~RecvSocket();

    //initializes socket to specific port
    //if no specific port stated - to standart
    //one, in Defines.h
    int Initialize(int port = -1);
    //recvs message, returns size of it
    //sets parameter as pointer to it
    int Recv(RecvStruct* out_result);

private:
    char *buffer_;
};

#endif // !RECVSOCKET_H

