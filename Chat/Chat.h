#pragma once

#include <vector>
#include <string>
#include <map>

#include "UserMsg.h"
#include "UserInfo.h"

#include "Mutex.h"
#include "Thread.h"

class Network;
class FileManager;

class Chat
{
public:
	Chat();
	~Chat();

	void SetNetwork(Network* net);
    void SetFM(FileManager* fm);

    void SetName(const std::string& name);
    const std::string& GetName()const; //name getter
    void SetColor(char color);
    const char GetColor()const; //color getter

	void SetUserInfo(char color, const std::string& name);
    
	void AddMsg(const UserMsg& msg, const std::string &name = "");

    void IOfflineMsg();	//sends online msg

    void IOnlineMsg();	//sends online msg

    int SendMsg(const UserMsg& msg); //broadcast message and AddMsg()
    int SendMsgTo(const std::string& name, UserMsg& msg); //all msgs user write goes directly to the chosen user

    const std::vector<UserMsg>& GetPrivateChatMsgs(const std::string &name)const;
    
    void ChangeOtherUserName(const std::string &from, const std::string &to);
    bool Load();

private:
    UserInfo user_info_;

    std::map<std::string, std::vector<UserMsg>*> messages_;

	Network* connected_network_;
    FileManager* FM_;

	Mutex chat_mutex_;

    void ClearMessages();

    void Save()const;
};