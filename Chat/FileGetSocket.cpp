#include "FileGetSocket.h"
#include "Defines.h"
#include "Parcer.h"

FileGetSocket::FileGetSocket(){}


FileGetSocket::~FileGetSocket(){}

bool FileGetSocket::Initialize()
{
    if (!TCPSocket::Initialize())
    {
        return false;
    }

    if(!SetListen())
    {
        return false;
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
    TCPSocket file_getter = Accept();
   
    if (!((CreateDirectory(DOWNLOAD_DIR, NULL)) ||
        (GetLastError() == ERROR_ALREADY_EXISTS)))
        return false;
 
    char buffer[CHUNK_SIZE] = {};

    file_getter.Recv(buffer, CHUNK_SIZE); //gettinh file name
    std::string dir(DOWNLOAD_DIR + std::string("\\") + std::string(buffer));

    HANDLE file = CreateFile(dir.c_str(),
                             GENERIC_WRITE,
                             0,
                             NULL,
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    BOOL err_check;
    DWORD bytes_written;
    while(true) //file getting
    {
        int recved_bytes = file_getter.Recv(buffer, CHUNK_SIZE);
        err_check = WriteFile(file,
                              buffer,
                              recved_bytes,
                              &bytes_written,
                              NULL);

        if(recved_bytes < CHUNK_SIZE)
        {
            break;
        }
    }

    CloseHandle(file);
    return err_check;
}

void FileGetSocket::GetList(std::vector<RecvFileInfo> &out_result)const
{
    TCPSocket list_getter = Accept();

    char buffer[FILE_LIST_MESSAGE_SIZE] = {};
    while(true)
    {
        int bytes_recved = list_getter.Recv(buffer, BUFFER_SIZE);

        if(!bytes_recved)
            break;

        UnpackedMessage msg = Parcer::UnpackMessage(buffer);

        out_result.push_back(*(RecvFileInfo*)msg.msg_);

        delete msg.msg_;
    }
}