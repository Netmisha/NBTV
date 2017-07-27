#ifndef SEND_FILE_INFO_H
#define SEND_FILE_INFO_H

#include <string>

struct SendFileInfo
{
    const std::string path_,    //path to file
                      name_,    //file name
                      ip_;      //ip to where send
};

#endif // !SEND_FILE_INFO_H