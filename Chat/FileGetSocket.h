#pragma once
#include "TCPSocket.h"
#include <iostream>
#include <string>
#include <vector>

#include "RecvFileInfo.h"

class FileGetSocket :
    public TCPSocket
{

public:
    FileGetSocket();
    ~FileGetSocket();

    bool Initialize();

    static unsigned GetFileStartup(void *this_ptr);
    void GetList(std::vector<RecvFileInfo> &out_result)const;
    bool GetFile();
};

