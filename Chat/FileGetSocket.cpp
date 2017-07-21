#include "FileGetSocket.h"
#include "Defines.h"

FileGetSocket::FileGetSocket():first_time(true){}


FileGetSocket::~FileGetSocket(){}

bool FileGetSocket::Initialize()
{
    if (!AbstractSocket::Initialize(TCP))
    {
        return false;
    }

    return true;
}

bool FileGetSocket::GetFile()
{
    if (first_time)
    {
        sockaddr_in sock_addr = { AF_INET, htons(FILE_PORT), INADDR_ANY };

        if (socket_ == INVALID_SOCKET)
        {
            std::cerr << "socket is not exist!\n";
            return false;
        }

        if (bind(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
        {
            std::cout << GetLastError();
            std::cerr << "unable to bind socket!\n";
            shutdown(socket_, 2);
        }

        if (listen(socket_, SOMAXCONN) != 0)
        {
            std::cerr << "unable to set listening socket mode!\n";
            shutdown(socket_, 2);
        }
        first_time = false;
    }
    SOCKET file_getter = accept(socket_, 0, 0);
    int ppop = WSAGetLastError();
   
    if (!((CreateDirectory("Download", NULL)) ||
        (GetLastError() == ERROR_ALREADY_EXISTS)))
        return false;
 
    char buffer[CHUNK_SIZE] = {};
   

    recv(file_getter, buffer, CHUNK_SIZE, 0); //gettinh file name
    std::string dir(("Download\\") + std::string(buffer));

    HANDLE file = CreateFile(dir.c_str(),
                             GENERIC_WRITE,
                             0,
                             NULL,
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    BOOL err_check;
    DWORD bytes_written;
    int die = 0;
    while (true) //file getting
    {
        int recved_bytes = recv(file_getter, buffer, CHUNK_SIZE, 0);
        err_check = WriteFile(file,
                                   buffer,
                                   recved_bytes,
                                   &bytes_written,
                                   NULL);
     
        if (recved_bytes < CHUNK_SIZE)
        {
            break;
        }
    }
    CloseHandle(file);
    return err_check;
}
