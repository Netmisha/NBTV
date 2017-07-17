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
	std::thread& GetInputThread();

	void ResetChat();
	void PutMsg(UserMsg msg);
	
	void AddMsg(UserMsg msg);
	void InputStream();

	void Activate();

private:
	void IOnlineMsg();
	void IOfflineMsg();
	void SendMsg(UserMsg msg);


	char msg_color_;
	std::string user_name_;
	std::string buffer_;

	std::vector< std::string > users_;
	std::vector< UserMsg > messages_;
	Network* connected_network_;

	std::thread input_thread_;
	std::mutex chat_mutex_;
};

void ActivateChat(Chat* chat);