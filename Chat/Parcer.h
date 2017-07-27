#ifndef PARCER_H
#define PARCER_H

#include "UnpackedMessage.h"
#include "Defines.h"

#include <vector>
#include <string>

enum ConstantSizes
{
    //1B flag + 4B int
    PREPARE_MESSAGE_SIZE = 5,   
    //1B flag, 1B type, 1B name size, 1B name color, 1B msg size
    CHAT_MESSAGE_HEADER_SIZE = 5,
    //1B flag + 1B type + 1B name size
    LOG_MESSAGE_HEADER_SIZE = 3,
	
	//1Bflag + 1Bindex
	GET_FILE_MESSAGE_SIZE = 5,

    FILE_LIST_REQUEST_SIZE = 1,

    FILE_LIST_MESSAGE_SIZE = 64
};

namespace Parcer
{


    //packs message and sets out_result as
    //pointer to it, message is allocated in heap
    //returns size of message
    int PackMessage(const MessageType &type, const void *in_msg, void* &out_result);
    //returns processed message from packet
    UnpackedMessage DLL_EXP UnpackMessage(const void *packet);


    int PackChatMessage(const void *in_msg, void* &out_packet);
    void* ParceChatMessage(const void *in_packet);

    int PackPrepMessage(const void *in_msg, void* &out_packet);
    void* ParcePrepMessage(const void *in_packet);

    int PackLogMessage(const void *in_msg, void* &out_packet);
    void* ParceLogMessage(const void *in_packet);

    int PackGetFileMessage(const void *in_msg, void* &out_packet);
    void* ParceGetFileMessage(const void *in_packet);

    int PackFileListRequest(const void *in_msg, void* &out_packet);
    //no need for parcer

    int PackFileList(const void *in_msg, void* &out_packet);
    void* ParceFileList(const void *in_packet);
};

#endif // !PARCER_H

