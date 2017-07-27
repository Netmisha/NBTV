#ifndef FILE_LIST_SEND_SOCKET_H
#define FILE_LIST_SEND_SOCKET_H

#include "TCPSocket.h"
#include "File.h"

#include <vector>

class FileListSendSocket : public TCPSocket
{
public:
    FileListSendSocket();
    ~FileListSendSocket();

    bool SendFileList(const std::vector<File> &to_send, const std::string &ip);
};

#endif // !FILE_LIST_SEND_SOCKET_H