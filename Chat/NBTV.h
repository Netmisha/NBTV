#pragma once

#define DLL_EXP __declspec(dllexport)

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

class AbstractSocket
{
public:
    AbstractSocket();
    AbstractSocket(const SOCKET &sock);
    virtual ~AbstractSocket() = 0;  //to make it abstract

    //initializes socket as
    //SOCK_STREAM/tcp or DGRAM/udp
    //depending on 'type'
    bool Initialize(const SocketConnectionType &type);

    //closes socket
    bool Close();

protected:
    SOCKET socket_;
};

struct RecvStruct
{
    //string with ip
    std::string ip_;
    //pointer to heap
    char *packet_ = NULL;

    RecvStruct() { ip_.resize(IP_SIZE); }
    ~RecvStruct() { delete[] packet_; }
};

class RecvSocket : public AbstractSocket
{
public:
    RecvSocket();
    ~RecvSocket();

    //initializes socket to specific port
    //if no specific port stated - to standart
    //one, in Defines.h
    bool Initialize(int port = -1);
    //recvs message, returns size of it
    //sets parameter as pointer to it
    int Recv(RecvStruct* out_result);

private:
    char *buffer_;
};


struct LogMessage
{
    LogType type_;
    std::string name_;
};

namespace Parcer
{
    UnpackedMessage DLL_EXP UnpackMessage(const void *packet);
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

