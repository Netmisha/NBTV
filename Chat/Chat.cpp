
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
	connected_network_->SendLogMsg(user_info_.GetName(), LOG_ONLINE);
}


void Chat::IOfflineMsg()
{
	connected_network_->SendLogMsg(user_info_.GetName(), LOG_OFFLINE);
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
    user_info_.SetName(name);
}

void Chat::SetUserInfo(char color, const std::string& name)
{
	user_info_.SetColor(color);
	user_info_.SetName(name);
	IOnlineMsg();
}

const std::string& Chat::GetName()const
{
    return user_info_.GetName();
}

const char Chat::GetColor()const
{
    return user_info_.GetColor();
}

void Chat::AddMsg(const UserMsg& msg, const std::string &name)
{
    std::string msg_chat = 
        (msg.type_ == PUBLIC ? PUBLIC_MSGS : (name.empty() ? msg.name_ : name));
    chat_mutex_.Lock();

    //if vector of user messages to this user do not exist
    if(messages_.find(msg_chat) == messages_.end())
        messages_[msg_chat] = new std::vector<UserMsg>;

    std::vector<UserMsg> *msgs = messages_[msg_chat];
    msgs->push_back(msg);

    if(msgs->size() > (msg.type_ == PUBLIC ?
                       MAX_PUBLIC_MSGS_STORED : MAX_PRIVATE_MSGS_STORED))
        msgs->erase(msgs->begin());

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

void Chat::SetColor(char color)
{
    user_info_.SetColor(color);
}

bool Chat::Load()
{
    return user_info_.Load();
}