#include "FileSendSocket.h"
#include "Defines.h"
#include <Mswsock.h>
#pragma comment(lib, "mswsock.lib")

FileSendSocket::FileSendSocket(){}

FileSendSocket::~FileSendSocket(){}

void FileSendSocket::Initialize() {}

bool FileSendSocket::SendFile(const std::string& pass,
                              const std::string& ip,
                              const std::string& filename,
                              unsigned int port)
{
    if(!AbstractSocket::Initialize(TCP))
    {
        return false;
    }

    if(!Connect(ip.c_str(), port))
    {
        return false;
    }

    Send(filename.c_str(), CHUNK_SIZE); //sending file name

    char buffer[CHUNK_SIZE];
    HANDLE file = CreateFile(pass.c_str(),
                             GENERIC_READ,
                             0,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    if(file == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    DWORD bytes_read = 0;
    BOOL err_check;
    while (true)
    {
        err_check = ReadFile(file,
                             buffer,
                             CHUNK_SIZE,
                             &bytes_read,
                             NULL);
        Send(buffer, bytes_read);
        if (bytes_read < CHUNK_SIZE)
            break;
    }

    CloseHandle(file);

    return err_check;
}
