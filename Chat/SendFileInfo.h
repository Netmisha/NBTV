#ifndef SEND_FILE_INFO_H
#define SEND_FILE_INFO_H

#include <string>

struct SendFileInfo
{
    const std::string path_,
                      name_,
                      ip_;
};

#endif // !SEND_FILE_INFO_H