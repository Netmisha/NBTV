#include "FileListSendSocket.h"

#include "Parcer.h"

FileListSendSocket::FileListSendSocket() {}

FileListSendSocket::~FileListSendSocket() {}

bool FileListSendSocket::SendFileList(const std::vector<File> &to_send,
                                      const std::string &ip,
                                      unsigned int port)
{
    if(!TCPSocket::Initialize())
    {
        return false;
    }

    if(!Connect(ip.c_str(), port))
    {
        return false;
    }

    for(File f : to_send)
    {
        void *send_msg = NULL;
        int msg_size = Parcer::PackMessage(FILE_LIST_MESSAGE, &f, send_msg);
        Send((const char*)send_msg, msg_size);

        delete[] send_msg;
    }

    //closing msg
    Send(NULL, 0);
    return true;
}