#ifndef PARCER_H
#define PARCER_H

#include "UserMsg.h"

enum
{
    PREPARE_MESSAGE = 0,
    CHAT_MESSAGE = 1
};

class Parcer
{
public:

    //packs message and sets out_result as
    //pointer to it, message is allocated in heap
    //returns size of message
    static int PackMessage(const UserMsg &user_msg, void* &out_result);
    //returns processed message from packet
    static UserMsg UnpackMessage(void *packet);
};

#endif // !PARCER_H

