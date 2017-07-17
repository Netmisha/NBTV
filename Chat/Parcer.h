#ifndef PARCER_H
#define PARCER_H

#include "UserMsg.h"
#include "LogMessage.h"
#include "UnpackedMessage.h"

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

