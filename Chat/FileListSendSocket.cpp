#include "FileListSendSocket.h"

#include "Parcer.h"

FileListSendSocket::FileListSendSocket() {}

FileListSendSocket::~FileListSendSocket() {}

bool FileListSendSocket::SendFileList(const std::vector<File> &to_send, const std::string &ip)
{
    if(!AbstractSocket::Initialize(TCP))
    {
        Close();
        return false;
    }

    sockaddr_in sock_addr;

    //set server port and IP
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(FILE_PORT); //Host TO Network Short
    sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //trying to connect 
    for(int i = 0; i < 10; ++i)
    {
        if(i == 9)
        {
            return false;
        }
        else if(connect(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
        {
            Sleep(1000);
        }
        else
            break;
    }

    for(File f : to_send)
    {
        void *to_send = NULL;
        int msg_size = Parcer::PackMessage(FILE_LIST_MESSAGE, &f, to_send);
        send(socket_, (const char*)to_send, msg_size, 0);

        delete[] to_send;
    }

    //closing msg
    send(socket_, NULL, 0, 0);
    Close();
    return true;
}