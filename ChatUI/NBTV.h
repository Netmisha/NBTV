#ifndef NBTV_H
#define NBTV_H
#pragma once

#define DLL_IMP __declspec(dllimport)

//size of ip in chars
#define IP_SIZE 16

#include <vector>
#include <string>

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
    const std::string& GetName();

    UnpackedMessage RecieveMessage();

    void* ActivateCommand(std::string& msg); //called by buttons with different commands
    //ChangeName, On/Off private mode, filelists, get someone`s file,
    //add/remove file, online users list, setcolor, exit
    const std::string GetIP()const;
    void PopBuffer(int num, std::string& buffer); //easy pop front

    void ActivatePrivateChat();
    void ExitPrivateChat();
    
    //returns true if name is used by other user
    //false otherwise
    bool IsNameUsed(const std::string &name)const;

    void EXIT();
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

    const std::string GetName()const;
    const std::string GetPath()const;
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