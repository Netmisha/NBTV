#ifndef UNPACKED_MESSAGE_H
#define UNPACKED_MESSAGE_H

enum MessageType
{
    //invalid type of message
    INVALID_TYPE = -1,
    //send once when app is starting
    PREPARE_MESSAGE = 0,
    //log online/offline or change name
    //as well as responce to online messages
    LOG_MESSAGE = 1,
    //simple chat message
    CHAT_MESSAGE = 2,
    //requesting send of file list
    FILE_LIST_REQUEST = 3,
    //message that it part of file list send
    FILE_LIST_MESSAGE = 4,
    //message requesting send of file
	GET_FILE_MESSAGE = 5,
    //heartbeat message
    HEARTBEAT_MESSAGE = 6
};

struct UnpackedMessage
{
    MessageType type_ = INVALID_TYPE;
    void *msg_ = 0;

    inline void Clear() { delete msg_; }
};

#endif // !UNPACKED_MESSAGE_H