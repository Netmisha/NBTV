#ifndef FILE_LIST_SEND_SOCKET_H
#define FILE_LIST_SEND_SOCKET_H

#include "AbstractSocket.h"
#include "File.h"

#include <vector>

class FileListSendSocket : public AbstractSocket
{
public:
    FileListSendSocket();
    ~FileListSendSocket();

    bool SendFileList(const std::vector<File> &to_send, const std::string &ip);
};

#endif // !FILE_LIST_SEND_SOCKET_H