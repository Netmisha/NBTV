#ifndef RECVSTRUCT_H
#define RECVSTRUCT_H

#include "Defines.h"

#include <string>
#include <memory>

struct RecvStruct
{
    //string with ip
    std::string ip_;
    //pointer to heap
    char *packet_ = NULL;

    RecvStruct() { ip_.resize(IP_SIZE); }
    ~RecvStruct() { delete[] packet_; }
};

#endif // !RECVSTRUCT_H