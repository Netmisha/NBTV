
#include "Network.h"
#include "Chat.h"

#include <iostream>
#include <chrono>
#include <sstream>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;


Chat::Chat() : input_is_working_(true){}


Chat::~Chat()
{
	connected_network_->StopNetwork();
}

void Chat::IOnlineMsg()
{
	connected_network_->SendLogMsg(user_name_, LOG_ONLINE);
	UserMsg msg = { msg_color_, user_name_, user_name_ + " is online!" };
	SendMsg(msg);
}

void Chat::IOfflineMsg()
{
	UserMsg msg = { msg_color_, user_name_, user_name_ + " left the chat!" };
	SendMsg(msg);
	cout << "\n Please wait... ";
	connected_network_->SendLogMsg(user_name_, LOG_OFFLINE);
}

void Chat::SendMsg(const UserMsg& msg)
{

	connected_network_->SendMsg(msg);
	buffer_.clear();
	AddMsg(msg);
}

int Chat::SendMsgTo(const std::string& name, UserMsg& msg)
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

void Chat::SetUserInfo(char color, const std::string& name)
{
	msg_color_ = color;
	user_name_ = name;
	IOnlineMsg();
}

Thread& Chat::GetInputThread()
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

void Chat::PutMsg(const UserMsg& msg)
{
    cout << msg.name_ << " : "; 

	cout << msg.msg_ << endl;
	
}

void Chat::AddMsg(const UserMsg& msg)
{
	chat_mutex_.Lock();

	messages_.push_back(msg);
	ResetChat();

	chat_mutex_.Unlock();
}

void Chat::InputStream()
{
	while (input_is_working_) 
	{
		ReadFromKeyboard();
		
		if (!CheckForCommands())
		{
			UserMsg msg = { msg_color_, user_name_, buffer_ };
			SendMsg(msg);
		}
	}
}

void Chat::ReadFromKeyboard()
{
	for (char tmp = _getch(); tmp != '\r'; tmp = _getch())
	{
		if (tmp == BACKSPACE_BUTTON && !buffer_.empty())
		{
			printf("\b \b");
			buffer_.pop_back();
		}
		else
		{
			buffer_.push_back(tmp);
			cout << tmp;
		}
	}
}

void Chat::PopBuffer(int num) //easy pop front
{
	for (int i = num; i > 0; i--)
	{
		buffer_.erase(buffer_.begin());
	}
}

bool Chat::CheckForCommands() //chat commands
{
	
	if (buffer_[0] == '/')
	{
		PopBuffer(1);

		if(!strncmp(buffer_.c_str(), "w ", 2))
		{
			PopBuffer(2);
			ActivatePrivateChat(buffer_);
		}
		else if (!strncmp(buffer_.c_str(), "setname ", 8))
		{
			PopBuffer(8);
			SetUserInfo(msg_color_, buffer_);
			buffer_.clear();
			ResetChat();
		}
		else if (!strncmp(buffer_.c_str(), "getf ", 5))
		{
			PopBuffer(5);
			std::stringstream stream;
			std::string name;
			int index;
			stream >> name;
			stream >> index;
			//connected_network_->GetFile(name, index);
			//set listen socket
		}
		else if (!strncmp(buffer_.c_str(), "setcolor ", 9))
		{
			PopBuffer(9);
			std::string color_str("COLOR 0");
			color_str += std::to_string( atoi( &buffer_.front() ) );
			system(color_str.c_str());
			buffer_.clear();
			ResetChat();
			//system("COLOR 07"); white 
		}
		else if (!strncmp(buffer_.c_str(), "exit", 4))
		{
			IOfflineMsg();
			input_is_working_ = false;

		}

		
		return true;
	}

	return false;
}

void Chat::Activate()
{
	input_thread_.BeginThread(ActivateChat, this);
}

void Chat::ActivatePrivateChat(std::string name) //all msgs user write goes directly to the chosen user
{
	buffer_.clear();
	AddMsg(UserMsg{ 7, "PRIVATE CHAT WITH", name });

	while (true)
	{
		ReadFromKeyboard();

		if (!strncmp(buffer_.c_str(), "/w", 2))
		{
			buffer_.clear();
			break;
		}
		UserMsg msg = { msg_color_, user_name_, buffer_ };
		if (SendMsgTo(name, msg) == -1)
		{
			AddMsg(UserMsg{ 7, "THRRE NO USER WITH THIS NAME ", name });
			break;
		}
	}
	ResetChat();
}

void ActivateChat(void* chat) //function for input thread
{
	((Chat*)chat)->InputStream();
}
