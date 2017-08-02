
#include "Network.h"
#include "Chat.h"

#include <iostream>
#include <chrono>
#include <sstream>
#include <conio.h>

Chat::Chat(){}

Chat::~Chat()
{
    ClearMessages();
}

void Chat::IOnlineMsg()
{
	connected_network_->SendLogMsg(user_name_, LOG_ONLINE);
}


void Chat::IOfflineMsg()
{
	connected_network_->SendLogMsg(user_name_, LOG_OFFLINE);
}

int Chat::SendMsg(const UserMsg& msg)
{
    AddMsg(msg);
	return connected_network_->SendMsg(msg);
}

int Chat::SendMsgTo(const std::string& name, UserMsg& msg)
{
	AddMsg(msg, name);    
    return connected_network_->SendMsgTo(name, msg);
}

const std::vector<UserMsg>& Chat::GetPrivateChatMsgs(const std::string &name)const
{
    return *messages_.find(name)->second;
}

void Chat::SetNetwork(Network * net)
{
	connected_network_ = net;
}

void Chat::SetFM(FileManager * fm)
{
    FM_ = fm;
}

void Chat::SetName(const std::string& name)
{
    user_name_ = name;
}

void Chat::SetUserInfo(char color, const std::string& name)
{
	msg_color_ = color;
	user_name_ = name;
	IOnlineMsg();
}

const std::string& Chat::GetName()const
{
    return user_name_;
}

const char Chat::GetColor()const
{
    return msg_color_;
}

void Chat::AddMsg(const UserMsg& msg, const std::string &name)
{
    std::string msg_chat = (msg.type_ == PUBLIC ? PUBLIC_MSGS : name);
    chat_mutex_.Lock();

    //if vector of user messages to this user do not exist
    if(messages_.find(msg_chat) == messages_.end())
        messages_[msg_chat] = new std::vector<UserMsg>;

    messages_[msg_chat]->push_back(msg);
	chat_mutex_.Unlock();
}

void Chat::ChangeOtherUserName(const std::string &from, const std::string &to)
{
    std::vector<UserMsg>* user_msgs = messages_[from];
    messages_.erase(from);
    messages_[to] = user_msgs;
}

void Chat::ClearMessages()
{
    for(std::pair<std::string, std::vector<UserMsg>*> pair : messages_)
    {
        delete pair.second;
    }
}