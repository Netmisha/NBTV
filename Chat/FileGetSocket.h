#pragma once
#include "TCPSocket.h"
#include <iostream>
#include <string>
#include <vector>

#include "RecvFileInfo.h"

class DLL FileGetSocket :
    public TCPSocket
{

public:
    FileGetSocket();
    ~FileGetSocket();

    bool Initialize(unsigned int listen_port = 0U);

    //thread startup function for GetFile
    static unsigned GetFileStartup(void *this_ptr);
    //returns vector of RecvFileInfo objects passed as parameter
    //needs to be called after requesting file list from client
    void GetList(std::vector<RecvFileInfo> &out_result)const;
    //gets previously requested file from other client
    bool GetFile();
};

