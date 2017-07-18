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
	connected_network_->SendLogMsg(user_name_, LOG_ONLINE);
	UserMsg msg = { msg_color_, user_name_, user_name_ + " is online!" };
	SendMsg(msg);
}

void Chat::IOfflineMsg()
{
	connected_network_->SendLogMsg(user_name_, LOG_OFFLINE);
	UserMsg msg = { msg_color_, user_name_, user_name_ + " left the chat!" };
	SendMsg(msg);
}

void Chat::SendMsg(UserMsg msg)
{
	connected_network_->SendMsg(msg);
	buffer_.clear();
	AddMsg(msg);
}

int Chat::SendMsgTo(std::string name, UserMsg msg)
{
	int result = connected_network_->SendMsgTo(name, msg);
	buffer_.clear();
	AddMsg(msg);

	return result;
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

const std::string& Chat::GetName()
{
    return  user_name_;
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
		
		for (char tmp = _getch(); tmp != '\r'; tmp = _getch())
		{	
			buffer_.push_back(tmp);
			cout << tmp;
		}
		if (buffer_[0] == '/' && buffer_[1] == 'w' && buffer_[2] == ' ')
		{
			for (int i = 3; i > 0; i--)
			{
				buffer_.erase(buffer_.begin());
			}

			ActivatePrivateChat(buffer_);

			continue;
		}
		UserMsg msg = { msg_color_, user_name_, buffer_ };
		SendMsg(msg);
		
	}
}

void Chat::Activate()
{
	input_thread_ = std::thread(ActivateChat, this);
}

void Chat::ActivatePrivateChat(std::string name) //all msgs user write goes directly to the chosen user
{
	buffer_.clear();
	AddMsg({ 7, "PRIVATE CHAT WITH", name });

	while (1)
	{
		for (char tmp = _getch(); tmp != '\r'; tmp = _getch())
		{
			buffer_.push_back(tmp);
			cout << tmp;
		}
		if (buffer_[0] == '/' && buffer_[1] == 'w')
		{
			buffer_.clear();
			break;
		}
		UserMsg msg = { msg_color_, user_name_, buffer_ };
		if (SendMsgTo(name, msg) == -1)
		{
			AddMsg({ 7, "THRRE NO USER WITH THIS NAME ", name });
			break;
		}
	}
}

void ActivateChat(Chat* chat) //function for input thread
{
	chat->InputStream();
}
