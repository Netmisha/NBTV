#pragma once

#include <vector>
#include <string>
#include <map>

#include "UserMsg.h"
#include "MyUserInfo.h"
#include "MessagesArchive.h"

#include "Mutex.h"
#include "Thread.h"

class Network;
class FileManager;

class Chat
{
public:
	Chat();
	~Chat();

    //set pointer to Network object
	void SetNetwork(Network* net);
    //set pointer to FileManager object
    void SetFM(FileManager* fm);

    //set user name
    void SetName(const std::string& name);
    //get user name
    const std::string& GetName()const;
    //set user color
    void SetColor(char color);
    //get user color
    const char GetColor()const;
    //set both color and name
	void SetUserInfo(char color, const std::string& name);
    //add message to MessagesArchive
	void AddMsg(const UserMsg& msg, const std::string &name = "");

    //send message and call AddMsg
    //returns number of bytes send
    //-1 if failed
    int SendMsg(const UserMsg& msg);
    //returns number of bytes send
    //-1 if failed
    //send msg to 'name' and call AddMsg
    int SendMsgTo(const std::string& name, UserMsg& msg);
    //get chat messages by 'name'
    //returns NULL there is none
    const std::vector<UserMsg>* GetChatMsgs(const std::string &name)const;
    //change user name in MessagesArchive
    void ChangeOtherUserName(const std::string &from, const std::string &to);
    //load user info
    //returns true if loaded successfully
    //false otherwise
    bool Load();

private:
    //user info is stored here
    MyUserInfo user_info_;
    //recent chat messages are stored here
    MessagesArchive messages_;

	Network* connected_network_;
    FileManager* FM_;
};