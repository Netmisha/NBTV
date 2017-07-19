#pragma once
#include "AbstractSocket.h"

#include <WS2tcpip.h>

#include <iostream>
#include <string>
class FileSendSocket : public AbstractSocket
{

public:
	FileSendSocket();
	~FileSendSocket();

	bool Initialize();
    bool SendFile(std::string pass, std::string ip);
};

