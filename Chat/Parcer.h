#ifndef PARCER_H
#define PARCER_H

#include "UserMsg.h"
#include "LogMessage.h"
#include "UnpackedMessage.h"

#include <vector>
#include <string>

enum ConstantSizes
{
    //1B flag + 4B int
    PREPARE_MESSAGE_SIZE = 5,   
    //1B flag, 1B name size, 1B name color, 1B msg size
    CHAT_MESSAGE_HEADER_SIZE = 4,
    //1B flag + 1B type + 1B name size
    LOG_MESSAGE_HEADER_SIZE = 3,
	
	//1Bflag + 1Bindex
	GET_FILE_MESSAGE_SIZE = 2,

    FILE_LIST_REQUEST_SIZE = 1,
    
    FILE_LIST_HEADER_SIZE = 3
};

class Parcer
{
public:

    //packs message and sets out_result as
    //pointer to it, message is allocated in heap
    //returns size of message
    static int PackMessage(const MessageType &type, const void *in_msg, void* &out_result);
    //returns processed message from packet
    static UnpackedMessage UnpackMessage(const void *packet);

private:
    static int PackChatMessage(const void *in_msg, void* &out_packet);
    static void* ParceChatMessage(const void *in_packet);

    static int PackPrepMessage(const void *in_msg, void* &out_packet);
    static void* ParcePrepMessage(const void *in_packet);

    static int PackLogMessage(const void *in_msg, void* &out_packet);
    static void* ParceLogMessage(const void *in_packet);

	static int PackGetFileMessage(const void *in_msg, void* &out_packet);
	static void* ParceGetFileMessage(const void *in_packet);

    static int PackFileListRequest(const void *in_msg, void* &out_packet);
        //no need for parcer
        
    static int PackFileList(const void *in_msg, void* &out_packet);
    static void* ParceFileList(const void *in_packet);
};

#endif // !PARCER_H

