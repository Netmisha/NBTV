#ifndef UNPACKED_MESSAGE_H
#define UNPACKED_MESSAGE_H

enum MessageType
{
    INVALID_TYPE = -1,
    PREPARE_MESSAGE = 0,
    LOG_MESSAGE = 1,
    CHAT_MESSAGE = 2
};

struct UnpackedMessage
{
    int type_ = INVALID_TYPE;
    void *msg_ = NULL;
};

#endif // !UNPACKED_MESSAGE_H