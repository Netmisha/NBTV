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

    bool SendFile(const std::string& pass, const std::string& ip, std::string& filename);

private:

    void Initialize();
};

