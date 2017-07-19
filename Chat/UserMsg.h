#ifndef USER_MSG_H
#define USER_MSG_H

#include <string>

struct UserMsg
{
    //color as enum
    char color_;
    //user name
    std::string name_;
    //user message
    std::string msg_;
};

#endif // !USER_MSG_H