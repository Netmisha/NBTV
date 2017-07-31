#pragma once

#define DLL_IMP __declspec(dllimport)

//size of ip in chars
#define IP_SIZE 16

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
    void *msg_ = 0;

    inline void Clear() { delete msg_; }
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

struct RecvStruct
{
    //string with ip
    std::string ip_;
    //pointer to heap
    char *packet_ = NULL;

    RecvStruct() { ip_.resize(IP_SIZE); }
    ~RecvStruct() {}

    inline void Clear() { delete[] packet_; }
};


struct LogMessage
{
    LogType type_;
    std::string name_;
};

namespace Parcer
{
    UnpackedMessage DLL_IMP UnpackMessage(const void *packet);
};

class DLL_IMP AppManager
{
public:
    //function to use from UI (.DLL)

    std::vector<UserMsg> GetCurrentChat();

    // void GetMsgLoop(); //message getting

    void SendFile(const std::string &path, const std::string &ip, const std::string &name);

    const std::string GetFilePath(int file_index)const;
    const std::string GetFileName(int file_index)const;

    void ProcessLogMessage(const LogMessage &msg, const std::string &ip);
    void SendList(const std::string& ip);

    void SendMsg(const std::string& msg); //after user wrote message in chat
    void AddMsg(const UserMsg& ms); //will be used by recv loop

    UnpackedMessage RecieveMessage();

    void* ActivateCommand(std::string& msg); //called by buttons with different commands
    //ChangeName, On/Off private mode, filelists, get someone`s file,
    //add/remove file, online users list, setcolor, exit
    const std::string GetIP()const;
    void PopBuffer(int num, std::string& buffer); //easy pop front

    void ActivatePrivateChat();
    void ExitPrivateChat();

    void EXIT();
};

DLL_IMP AppManager app_man;