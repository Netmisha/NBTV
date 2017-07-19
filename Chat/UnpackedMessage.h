#ifndef UNPACKED_MESSAGE_H
#define UNPACKED_MESSAGE_H

enum MessageType
{
    INVALID_TYPE = -1,
    PREPARE_MESSAGE = 0,
    LOG_MESSAGE = 1,
    CHAT_MESSAGE = 2,
    FILE_LIST_REQUEST = 3,
    FILE_LIST_MESSAGE = 4,
	GET_FILE_MESSAGE = 5
};

struct UnpackedMessage
{
    int type_ = INVALID_TYPE;
    void *msg_ = NULL;
};

#endif // !UNPACKED_MESSAGE_H