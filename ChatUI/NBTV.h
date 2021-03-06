#ifndef NBTV_H
#define NBTV_H
#pragma once

#define DLL_IMP __declspec(dllimport)

//size of ip in chars
#define IP_SIZE 16

#include <vector>
#include <string>

const std::string PUBLIC_MSGS("PUBLIC_MSGS");

typedef unsigned(*thread_function)(void*);

enum ChatMsgType
{
    PUBLIC = 0,
    PRIVATE = 1
};

enum LogType
{
    LOG_INVALID_TYPE = (unsigned char)-1,
    LOG_OFFLINE = (unsigned char)0,
    LOG_ONLINE = (unsigned char)1,
    LOG_UPDATE = (unsigned char)2,
    LOG_RESPONCE = (unsigned char)3
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

struct LogMessage
{
    LogType type_;
    std::string name_;
    std::string prev_name_;
};

class DLL_IMP Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock()const;
    void Unlock()const;
    //tries to lock the thread
    //returns 1 if success
    //0 otherwise, don't block thread execution
    //-1 if mutex is not valid
    int TryLock()const;

    bool IsValid()const;
};

class DLL_IMP Thread
{
public:
    Thread();
    Thread(thread_function func, void *params);
    ~Thread();

    //starts thread with specific function
    //and vprt to params
    bool BeginThread(thread_function func, void *params);
    //joines thread
    void Join()const;
};

struct UserInfo
{
    std::string user_name_;
    char color;
};

class DLL_IMP AppManager
{
public:
    //returns refference to vector of user msgs of specific private chat
    //use PUBLIC_MSGS constant for getting public messages
    const std::vector<UserMsg>* GetPrivateChatMsgs(const std::string &name)const;

    //get specific yours shared file path and name
    const std::string& GetFilePath(int file_index)const;
    const std::string& GetFileName(int file_index)const;
    //send list to specific ip
    void SendList(const std::string& ip, unsigned int port = 0U);

    //send msg to everyone
    int SendMsg(const std::string& msg);
    //send msg to someone specific
    int SendMsgTo(const std::string& msg, const std::string &name);
    //get your name
    const std::string& GetName()const;
    const char GetColor() const;

    //recieve message from broadcasts
    UnpackedMessage RecieveMessage();

    void* ActivateCommand(std::string& msg); //called by buttons with different commands
    //ChangeName, On/Off private mode, filelists, get someone`s file,
    //add/remove file, online users list, setcolor, exit
    //get your ip
    const std::string GetIP()const;
    void PopBuffer(int num, std::string& buffer); //easy pop front

    void ActivatePrivateChat();
    void ExitPrivateChat();

    //returns true if name is used by other user
    //false otherwise
    bool IsNameUsed(const std::string &name)const;
    //load your info from file
    //returns true if successful
    bool LoadUserInfo();

    void GetFile(const std::string &name, int index);

    void StopNetwork();

    void SetUserInfo(const std::string &name, char color);
    void SetUserInfo(const std::string &name);
    void SetUserInfo(char color);

    bool IsUserlistInvalid();
};

class DLL_IMP File
{
    //private members for stl
private:
    std::string name_,
                path_;
    double size_KB_;

public:
    //true if path is value
    //false otherwise
    bool SetFile(const std::string &path);
    bool IsValid()const;

    const std::string& GetName()const;
    const std::string& GetPath()const;
    const double GetSizeKB()const;
    const double GetSizeMB()const;
};


struct RecvFileInfo
{
    std::string name_;
    double size_KB_;
};

DLL_IMP AppManager app_man;

#endif // !NBTV_H