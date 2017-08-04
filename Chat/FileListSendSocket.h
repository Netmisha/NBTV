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

    //sends vector of File objects to specific ip
    //sending is done through connected tcp sockets
    //and numerous message sends, so requester needs 
    //to have listening socket up to send him list
    bool SendFileList(const std::vector<File> &to_send,
                      const std::string &ip,
                      unsigned int port = 0U);
};

#endif // !FILE_LIST_SEND_SOCKET_H