#include "FileSendSocket.h"
#include "Defines.h"
#include <Mswsock.h>
#pragma comment(lib, "mswsock.lib")

FileSendSocket::FileSendSocket(){}

FileSendSocket::~FileSendSocket(){}

void FileSendSocket::Initialize() {}

bool FileSendSocket::SendFile(const std::string& pass, const std::string& ip, const std::string& filename)
{
    if(!AbstractSocket::Initialize(TCP))
    {
        return false;
    }

    sockaddr_in sock_addr;

    //set server port and IP
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(FILE_PORT); //Host TO Network Short
    sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());


    //trying to connect 
    for (int i = 0; i < 10; ++i)
    {
        if (connect(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
        {
            std::cerr << "Unable to connect!\n";
            Sleep(1000);
        }
        else
            break;
    }
   

    send(socket_, filename.c_str(), CHUNK_SIZE, 0); //sending file name

    char buffer[CHUNK_SIZE];
    HANDLE file = CreateFile(pass.c_str(),
                             GENERIC_READ,
                             0,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    DWORD bytes_read = 0;
    BOOL err_check;
    while (true)
    {
        err_check = ReadFile(file,
                             buffer,
                             CHUNK_SIZE,
                             &bytes_read,
                             NULL);
        send(socket_, buffer, bytes_read, 0);
        if (bytes_read < CHUNK_SIZE)
            break;
    }

    Close();
    CloseHandle(file);

    return err_check;
}
