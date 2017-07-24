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

    static unsigned GetFileStartup(void *this_ptr);
    bool GetFile();
};

