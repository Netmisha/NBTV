#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include <string>

enum LogType
{
    LOG_INVALID_TYPE = (unsigned char)-1,
    LOG_OFFLINE = (unsigned char)0,
    LOG_ONLINE = (unsigned char)1,
    LOG_UPDATE = (unsigned char)2,
    LOG_RESPONCE = (unsigned char)3
};

struct LogMessage
{
    LogType type_;
    std::string name_;
    std::string prev_name_;
    char color_;

    inline LogMessage(const LogType &type = LOG_INVALID_TYPE,
                      const std::string &name = "",
                      const std::string &prev_name = "",
                      char color = -1)
                      : type_(type), name_(name), prev_name_(prev_name), color_(color){}
};

#endif // !LOG_MESSAGE_H