#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include <string>

enum LogType
{
    LOG_OFFLINE = (unsigned char)0,
    LOG_ONLINE = (unsigned char)1,
    LOG_UPDATE = (unsigned char)2
};

struct LogMessage
{
    LogType type_;
    std::string name_;
};

#endif // !LOG_MESSAGE_H