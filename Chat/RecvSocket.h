#ifndef RECVSOCKET_H
#define RECVSOCKET_H

#include "AbstractSocket.h"
#include "RecvStruct.h"
#include "Defines.h"

#include <WS2tcpip.h>

class DLL RecvSocket : public AbstractSocket
{
public:
    RecvSocket();
    ~RecvSocket();

    //initializes socket to specific port
    //if no specific port stated - to standart
    //one, in Defines.h
    bool Initialize(unsigned int port = 0U);
    //recvs message, returns size of it
    //sets parameter as pointer to it
    int Recv(RecvStruct* out_result)const;

private:
    char *buffer_;
};

#endif // !RECVSOCKET_H

