#ifndef UNPACKED_MESSAGE_H
#define UNPACKED_MESSAGE_H

enum
{
    INVALID_TYPE = -1,
    PREPARE_MESSAGE = 0,
    LOG_MESSAGE = 1,
    CHAT_MESSAGE = 2

};

enum
{
    PREPARE_MESSAGE_SIZE = 5,
    CHAT_MESSAGE_HEADER_SIZE = 4,
    LOG_MESSAGE_SIZE = 2,
    LOG_ONLINE = (unsigned char)1,
    LOG_OFFLINE = (unsigned char)0,
    LOG_MESSAGE_HEADER_SIZE = 2,

};

struct UnpackedMessage
{
    int type_ = INVALID_TYPE;
    void *msg_ = NULL;
};

#endif // !UNPACKED_MESSAGE_H