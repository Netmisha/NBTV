#ifndef MY_USER_INFO_H
#define MY_USER_INFO_H

#include "UserInfo.h"

#include <string>

class MyUserInfo
{
public:
    MyUserInfo();
    ~MyUserInfo();

    bool Load();
    void Save()const;

    const std::string& GetName()const;
    void SetName(const std::string &name);
    char GetColor()const;
    void SetColor(char color);

private:
    UserInfo my_user_info_;
};

#endif // !MY_USER_INFO_H