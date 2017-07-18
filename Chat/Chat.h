#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <thread>

#include "UserMsg.h"

class Network;


class Chat
{
public:
	Chat();
	~Chat();

	void SetNetwork(Network* net);
	void SetUserInfo(char color, std::string name);
	std::thread& GetInputThread(); //thread descriptor getter

	void ResetChat();
	void PutMsg(UserMsg msg); // show msg to the screen
	
	void AddMsg(UserMsg msg); //adds msg to the vector and call`s PutMsg()
	void InputStream();  

	void Activate(); //starts input thread
	void ActivatePrivateChat(std::string name); //private chat input mode

private:
	void IOnlineMsg();	//sends online msg
	void IOfflineMsg();	//sends online msg
	void SendMsg(UserMsg msg); //broadcast message and AddMsg()
	int SendMsgTo(std::string name, UserMsg msg); //all msgs user write goes directly to the chosen user


	char msg_color_;
	std::string user_name_;
	std::string buffer_;

	std::vector< std::string > users_;
	std::vector< UserMsg > messages_;
	Network* connected_network_;

	std::thread input_thread_;
	std::mutex chat_mutex_;
};

void ActivateChat(Chat* chat); //function for input thread