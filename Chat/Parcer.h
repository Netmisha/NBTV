#ifndef PARCER_H
#define PARCER_H

#include "UnpackedMessage.h"
#include "Defines.h"

#include "File.h"
#include "UserMsg.h"
#include "LogMessage.h"
#include "RecvFileInfo.h"

enum ConstantSizes
{
    //1B flag + 4B int
    PREPARE_MESSAGE_SIZE = 5,   
    //1B flag + 1B type + 1B name size + 1B name color + 1B msg size
    CHAT_MESSAGE_HEADER_SIZE = 5,
    //1B flag + 1B type + 1B name size + 1B color + 1B prev name size
    LOG_MESSAGE_HEADER_SIZE = 5,
	//1B flag + 4B index
	GET_FILE_MESSAGE_SIZE = 5,
    //1B flag
    FILE_LIST_REQUEST_SIZE = 1,
    //static size to not screw over
    //sending list by numerous messages
    FILE_LIST_MESSAGE_SIZE = 64
};

namespace Parcer
{
    //packs message and sets out_result as
    //pointer to it, message is allocated in heap as char array
    //so use delete[] to clear it
    //returns size of message
    DLL int PackMessage(const MessageType &type, const void *in_msg, void* &out_result);
    //returns processed message from packet
    DLL UnpackedMessage UnpackMessage(const void *packet);

    //used to pack chat messages (in_msg is ptr to UserMsg)
    int PackChatMessage(const void *in_msg, void* &out_packet);
    //returns ptr to UserMsg
    void* ParceChatMessage(const void *in_packet);

    //used to pack prepare message (in_msg is ptr to int as random value)
    int PackPrepMessage(const void *in_msg, void* &out_packet);
    //returns ptr to int
    void* ParcePrepMessage(const void *in_packet);

    //used to pack log message (in_msg is ptr to LogMessage)
    int PackLogMessage(const void *in_msg, void* &out_packet);
    //returns ptr to LogMessage
    void* ParceLogMessage(const void *in_packet);

    //used to pack get file request message (in_msg is int as file index)
    int PackGetFileMessage(const void *in_msg, void* &out_packet);
    //returns ptr to int
    void* ParceGetFileMessage(const void *in_packet);

    //used to pack file list request (in_msg is NULL)
    int PackFileListRequest(const void *in_msg, void* &out_packet);
    //no need for parcer

    //used to pack single file for sending as part
    //of sending whole file list (in_msg is ptr to File object)
    int PackFileList(const void *in_msg, void* &out_packet);
    //returns ptr to File
    void* ParceFileList(const void *in_packet);
};

#endif // !PARCER_H

