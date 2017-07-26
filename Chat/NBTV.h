#pragma once

#define DLL_EXP __declspec(dllexport)

#include <vector>
#include <string>

enum ChatMsgType
{
    PUBLIC = 0,
    PRIVATE = 1
};

enum LogType
{
    LOG_OFFLINE = (unsigned char)0,
    LOG_ONLINE = (unsigned char)1,
    LOG_UPDATE = (unsigned char)2
};

struct UserMsg
{
    ChatMsgType type_;
    //color as enum
    char color_;
    //user name
    std::string name_;
    //user message
    std::string msg_;
};

struct LogMessage
{
    LogType type_;
    std::string name_;
};

class AppManager
{
public:
    void DLL_EXP Work();
    DLL_EXP AppManager();
    DLL_EXP ~AppManager();



    //function to use from UI (.DLL)

    std::vector<UserMsg> DLL_EXP GetCurrentChat();

    // void GetMsgLoop(); //message getting

    void DLL_EXP SendFile(const std::string &path, const std::string &ip, const std::string &name);

    const std::string DLL_EXP GetFilePath(int file_index)const;
    const std::string GetFileName(int file_index)const;

    void DLL_EXP ProcessLogMessage(const LogMessage &msg, const std::string &ip);
    void DLL_EXP SendList(const std::string& ip);

    void DLL_EXP SendMsg(const std::string& msg); //after user wrote message in chat
    void DLL_EXP AddMsg(const UserMsg& ms); //will be used by recv loop

    void DLL_EXP *ActivateCommand(std::string& msg); //called by buttons with different commands
    //ChangeName, On/Off private mode, filelists, get someone`s file,
    //add/remove file, online users list, setcolor, exit
    const DLL_EXP std::string GetIP()const;
    void DLL_EXP PopBuffer(int num, std::string& buffer); //easy pop front

    void DLL_EXP ActivatePrivateChat();
    void DLL_EXP ExitPrivateChat();

    void DLL_EXP EXIT();
};

