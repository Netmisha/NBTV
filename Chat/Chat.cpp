#include "Chat.h"
#include "Network.h"
#include <iostream>
#include <thread>
#include <chrono>

#include <conio.h>

using std::cout;
using std::cin;
using std::endl;


Chat::Chat(){}


Chat::~Chat()
{
	IOfflineMsg();
}

void Chat::IOnlineMsg()
{
	UserMsg msg = { msg_color_, user_name_, user_name_ + " is online!" };
	SendMsg(msg);
}

void Chat::IOfflineMsg()
{
	UserMsg msg = { msg_color_, user_name_, user_name_ + " left the chat!" };
	SendMsg(msg);
}

void Chat::SendMsg(UserMsg msg)
{
	connected_network_->SendMsg(msg);
	buffer_.clear();
	AddMsg(msg);
}

void Chat::SetNetwork(Network * net)
{
	connected_network_ = net;
}

void Chat::SetUserInfo(char color, std::string name)
{
	msg_color_ = color;
	user_name_ = name;
	IOnlineMsg();
}

std::thread& Chat::GetInputThread()
{
	return input_thread_;
}

void Chat::ResetChat()
{
	system("cls");
	for (auto i : messages_)
	{
		PutMsg(i);
	}
	std::cerr << "Please enter message: " << buffer_;
	
}

void Chat::PutMsg(UserMsg msg)
{
	

	//std::string color_str("COLOR 0");
	//color_str += std::to_string(msg.color_);
	//system(color_str.c_str());
	//	system("COLOR 07");

    cout << msg.name_ << " : "; 

	cout << msg.msg_ << endl;
	
	
	
}

void Chat::AddMsg(UserMsg msg)
{
	chat_mutex_.lock();

	messages_.push_back(msg);
	ResetChat();

	chat_mutex_.unlock();
}

void Chat::InputStream()
{
	while (1)
	{
		
		
		char tmp;
		for (tmp = _getch(); tmp != '\r'; tmp = _getch())
		{	
			buffer_.push_back(tmp);
			cout << tmp;
		}
		UserMsg msg = { msg_color_, user_name_, buffer_ };
		SendMsg(msg);
		
	}
}

void Chat::Activate()
{
	input_thread_ = std::thread(ActivateChat, this);
}

void ActivateChat(Chat* chat)
{
	chat->InputStream();
}
