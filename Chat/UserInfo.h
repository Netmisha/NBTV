#ifndef USER_INFO_H
#define USER_INFO_H

#include <string>

struct UserInfo
{
    std::string user_name_;
    char color_;

    inline bool operator==(const UserInfo& info) { return (user_name_ == info.user_name_); }
};

#endif // !USER_INFO_H