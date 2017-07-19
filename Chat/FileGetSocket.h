#pragma once
#include "AbstractSocket.h"
#include <iostream>
#include <string>

class FileGetSocket :
    public AbstractSocket
{
public:
    FileGetSocket();
    ~FileGetSocket();

    bool Initialize();

    void GetFile(std::string filename);
};

