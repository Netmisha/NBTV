
#include "Network.h"
#include "Chat.h"

#include <iostream>
#include <chrono>
#include <sstream>
#include <conio.h>

Chat::Chat(){}

Chat::~Chat(){}

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

const std::vector<UserMsg>* Chat::GetChatMsgs(const std::string &name)const
{
    return messages_.GetUserMsgs(name);
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
    messages_.AddMsg(msg, name);
}

void Chat::ChangeOtherUserName(const std::string &from, const std::string &to)
{
    messages_.ChangeUsername(from, to);
}

void Chat::SetColor(char color)
{
    user_info_.SetColor(color);
}

bool Chat::Load()
{
    return user_info_.Load();
}