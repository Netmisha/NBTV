#pragma once

#include "Network.h"
#include "Chat.h"
#include "FileManager.h"
#include <vector>

class DLL AppManager
{
	Chat chat_;
	Network network_;
    FileManager fm_;

public:
	AppManager(unsigned int broadc_port = 0U, unsigned int tcp_port = 0U);
    ~AppManager();

    //returns refference to vector of user msgs of specific private chat
    //use PUBLIC_MSGS constant for getting public messages
    const std::vector<UserMsg>& GetPrivateChatMsgs(const std::string &name)const;

    //get specific yours shared file path and name
    const std::string& GetFilePath(int file_index)const;
    const std::string& GetFileName(int file_index)const;
    //send list to specific ip
    void SendList(const std::string& ip, unsigned int port = 0U);

    //send msg to everyone
    int SendMsg(const std::string& msg); //after user wrote message in chat
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
};