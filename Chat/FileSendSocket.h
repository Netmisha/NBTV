#pragma once
#include "TCPSocket.h"

#include <WS2tcpip.h>

#include <iostream>
#include <string>
class DLL FileSendSocket : public TCPSocket
{
public:
	FileSendSocket();
	~FileSendSocket();

    bool SendFile(const std::string& pass,
                  const std::string& ip,
                  const std::string& filename,
                  unsigned int port = 0);

private:

    void Initialize();
};

