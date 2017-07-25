#include "FileGetSocket.h"
#include "Defines.h"
#include "Parcer.h"

FileGetSocket::FileGetSocket(){}


FileGetSocket::~FileGetSocket(){}

bool FileGetSocket::Initialize()
{
    if (!AbstractSocket::Initialize(TCP))
    {
        return false;
    }

    sockaddr_in sock_addr = { AF_INET, htons(FILE_PORT), INADDR_ANY };

    if(socket_ == INVALID_SOCKET)
    {
        std::cerr << "socket is not exist!\n";
        return false;
    }

    if(bind(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
    {
        std::cout << GetLastError();
        std::cerr << "unable to bind socket!\n";
        Close();
    }

    if(listen(socket_, SOMAXCONN) != 0)
    {
        std::cerr << "unable to set listening socket mode!\n";
        Close();
    }
    return true;
}

unsigned FileGetSocket::GetFileStartup(void *this_ptr)
{
   (*(FileGetSocket*)this_ptr).GetFile();
    return 0;
}

bool FileGetSocket::GetFile()
{
    SOCKET file_getter = accept(socket_, 0, 0);
   
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

void FileGetSocket::GetList(std::vector<RecvFileInfo> &out_result)const
{
    SOCKET list_getter = accept(socket_, 0, 0);

    char buffer[FILE_LIST_MESSAGE_SIZE] = {};
    while(true)
    {
        int bytes_recved = recv(list_getter, buffer, BUFFER_SIZE, 0);

        if(!bytes_recved)
            break;

        UnpackedMessage msg = Parcer::UnpackMessage(buffer);

        out_result.push_back(*(RecvFileInfo*)msg.msg_);

        delete msg.msg_;
    }

    closesocket(list_getter);
}