#ifndef PARCER_H
#define PARCER_H

#include "UserMsg.h"
#include "LogMessage.h"
#include "UnpackedMessage.h"

enum
{
    //1B flag + 4B int
    PREPARE_MESSAGE_SIZE = 5,   
    //1B flag, 1B name size, 1B name color, 1B msg size
    CHAT_MESSAGE_HEADER_SIZE = 4,

    LOG_ONLINE = (unsigned char)1,
    LOG_OFFLINE = (unsigned char)0,
    //1B flag + 1B type + 1B name size
    LOG_MESSAGE_HEADER_SIZE = 3,
};

class Parcer
{
public:

    //packs message and sets out_result as
    //pointer to it, message is allocated in heap
    //returns size of message
    static int PackMessage(int type, void *in_msg, void* &out_result);
    //returns processed message from packet
    static UnpackedMessage UnpackMessage(void *packet);

private:
    static int PackChatMessage(void *in_msg, void* &out_packet);
    static void* ParceChatMessage(void *in_packet);

    static int PackPrepMessage(void *in_msg, void* &out_packet);
    static void* ParcePrepMessage(void *in_packet);

    static int PackLogMessage(void *in_msg, void* &out_packet);
    static void* ParceLogMessage(void *in_packet);
};

#endif // !PARCER_H

