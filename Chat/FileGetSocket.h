#pragma once
#include "AbstractSocket.h"
#include <iostream>
#include <string>

class FileGetSocket :
    public AbstractSocket
{
    bool first_time;

public:
    FileGetSocket();
    ~FileGetSocket();

    bool Initialize();

    bool GetFile();
};

