#pragma once

#include <vector>
#include <string>

#include "UserMsg.h"

#include "Mutex.h"
#include "Thread.h"

class Network;


class Chat
{
public:
	Chat();
	~Chat();

	void SetNetwork(Network* net);
	void SetUserInfo(char color, const std::string& name);
	Thread& GetInputThread(); //thread descriptor getter
    const std::string& GetName(); //name getter

	void ResetChat();
	void PutMsg(const UserMsg& msg); // show msg to the screen
	
	void AddMsg(const UserMsg& msg); //adds msg to the vector and call`s PutMsg()
	void InputStream(); 
	void ReadFromKeyboard(); //reading from keyboard 
	void PopBuffer(int num);
	bool CheckForCommands();

	void Activate(); //starts input thread
	void ActivatePrivateChat(std::string name); //private chat input mode

	
private:
	void IOnlineMsg();	//sends online msg
	void IOfflineMsg();	//sends online msg
	void SendMsg(const UserMsg& msg); //broadcast message and AddMsg()
	int SendMsgTo(const std::string& name, UserMsg& msg); //all msgs user write goes directly to the chosen user


	char msg_color_;
	std::string user_name_;
	std::string buffer_;

	std::vector< std::string > users_;
	std::vector< UserMsg > messages_;
	Network* connected_network_;

	volatile bool input_is_working_;
	Thread input_thread_;
	Mutex chat_mutex_;
};

unsigned ActivateChat(void* chat); //function for input thread