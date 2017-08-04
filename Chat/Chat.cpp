
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
    Save();
}

void Chat::IOnlineMsg()
{
	connected_network_->SendLogMsg(user_info_.user_name_, LOG_ONLINE);
}


void Chat::IOfflineMsg()
{
	connected_network_->SendLogMsg(user_info_.user_name_, LOG_OFFLINE);
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
    user_info_.user_name_ = name;
}

void Chat::SetUserInfo(char color, const std::string& name)
{
	user_info_.color_ = color;
	user_info_.user_name_ = name;
	IOnlineMsg();
}

const std::string& Chat::GetName()const
{
    return user_info_.user_name_;
}

const char Chat::GetColor()const
{
    return user_info_.color_;
}

void Chat::AddMsg(const UserMsg& msg, const std::string &name)
{
    std::string msg_chat = (msg.type_ == PUBLIC ? PUBLIC_MSGS : name);
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

void Chat::Save()const
{
    //if directory to save shared files data
    //doesn't exist -> create it
    if((CreateDirectory(DATA_SAVE_DIR, NULL)) ||
       (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        HANDLE file = CreateFile(USER_DATA_SAVE_FULLPATH,   //path
                                 GENERIC_WRITE,             //to write
                                 0,                         //non-share
                                 NULL,                      //security
                                 CREATE_ALWAYS,             //always create
                                 FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                                 NULL);                     //why no default arguments
        //if failed to create file
        if(file == INVALID_HANDLE_VALUE)
        {
            return;
        }

        BOOL err_check;
        do
        {
            int bytes_written;
            err_check = WriteFile(file,
                                       &user_info_.color_,
                                       sizeof(user_info_.color_),
                                       (DWORD*)&bytes_written,
                                       NULL);

            if(!err_check)
                break;

            unsigned char size = (unsigned char)user_info_.user_name_.length();
            err_check = WriteFile(file,
                                       &size,
                                       sizeof(size),
                                       (DWORD*)&bytes_written,
                                       NULL);

            if(!err_check)
                break;

            err_check = WriteFile(file,
                                       &user_info_.user_name_[0],
                                       size,
                                       (DWORD*)&bytes_written,
                                       NULL);
        } while(false);

        CloseHandle(file);

        //if writing failed - delete file
        //as it is corrupted
        if(!err_check)
            DeleteFileA(USER_DATA_SAVE_FULLPATH);
    }
}

bool Chat::Load()
{
    HANDLE file = CreateFile(FILE_DATA_SAVE_FULLPATH,   //path
                             GENERIC_READ,              //to read
                             0,                         //non-share
                             NULL,                      //security
                             OPEN_EXISTING,             //only existing
                             FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                             NULL);
    //if there is no existing file
    if(file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BOOL error_flag;
    do
    {
        DWORD bytes_read = 0;
        error_flag = ReadFile(file,
                              &user_info_.color_,
                              sizeof(user_info_.color_),
                              &bytes_read,
                              NULL);
        if(!error_flag)
            break;

        int size;
        error_flag = ReadFile(file,
                              &size,
                              sizeof(size),
                              &bytes_read,
                              NULL);
        if(!error_flag)
            break;

        user_info_.user_name_.resize(size);
        error_flag = ReadFile(file,
                              &user_info_.user_name_[0],
                              size,
                              &bytes_read,
                              NULL);
    } while(false);

    if(!error_flag)
    {
        user_info_.color_ = DEFAULT_COLOR;
        user_info_.user_name_ = "";
    }

    CloseHandle(file);
    return error_flag;
}

void Chat::SetColor(char color)
{
    user_info_.color_ = color;
}