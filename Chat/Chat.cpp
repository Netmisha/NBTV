
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

Chat::~Chat(){}

void Chat::IOnlineMsg()
{
	connected_network_->SendLogMsg(user_name_, LOG_ONLINE);
	UserMsg msg = { PUBLIC, msg_color_, user_name_, user_name_ + " is online!" };
	SendMsg(msg);
}

void Chat::IChangedName(std::string& old_name)
{
    connected_network_->SendLogMsg(user_name_, LOG_UPDATE);
    UserMsg msg = { PUBLIC, msg_color_, user_name_, old_name + " changed name to " + user_name_ + " !"};
    SendMsg(msg);
}

void Chat::IOfflineMsg()
{
	UserMsg msg = { PUBLIC, msg_color_, user_name_, user_name_ + " left the chat!" };
	SendMsg(msg);
	cout << "\n Please wait... ";
	connected_network_->SendLogMsg(user_name_, LOG_OFFLINE);
}

void Chat::PrepareSendMsg(std::string& str_msg)
{
    UserMsg msg = { PUBLIC, msg_color_, user_name_, str_msg };
    SendMsg(msg);
}

void Chat::PrepareAddMsg(std::string& str_msg)
{
    UserMsg msg = { PUBLIC, msg_color_, user_name_, str_msg };
    AddMsg(msg);
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

std::vector<UserMsg>& Chat::GetCurrentChat()
{
    return messages_;
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

Thread& Chat::GetInputThread()
{
	return input_thread_;
}

const std::string& Chat::GetName()
{
    return  user_name_;
}

void Chat::ResetChat() const
{
	system("cls");
	for (auto i : messages_)
	{
		PutMsg(i);
	}
	std::cerr << "Please enter message: " << buffer_;
	
}

void Chat::PutMsg(const UserMsg& msg) const
{
    cout << msg.name_ << " : "; 

	cout << msg.msg_ << endl;
	
}


void Chat::PrintMyList(std::vector<File>& list) const
{
    chat_mutex_.Lock();
    cout << endl;
    for (size_t i = 0; i < list.size(); i++)
    {
        cout << i + 1 << " \t" + list[i].GetName() << " \t\t " << list[i].GetSizeMB() << "MB" << endl;
    }
    std::cerr << "Please enter message: " << buffer_;

    chat_mutex_.Unlock();
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
			UserMsg msg = { PUBLIC, msg_color_, user_name_, buffer_ };
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

        if (!strncmp(buffer_.c_str(), "w ", 2))
        {
            PopBuffer(2);
            ActivatePrivateChat(buffer_);
        }
        else if (!strncmp(buffer_.c_str(), "setname ", 8))
        {
            PopBuffer(8);
            std::string old_name = user_name_;
            user_name_ = buffer_;
            IChangedName(old_name);
            buffer_.clear();
            ResetChat();
        }
        else if (!strncmp(buffer_.c_str(), "fl ", 3))
        {
            PopBuffer(3);
            std::stringstream stream(buffer_);
            std::string name;
            stream >> name;
            buffer_.clear();
            ResetChat();
            if (name.empty())
            {
                std::vector<File> list;
                FM_->GetFiles(list);
                PrintMyList(list); //I print my list
            }
            else
            {
                connected_network_->RequestSomeoneList(name); //asking for someone`s list

                std::vector<RecvFileInfo> *list = new std::vector<RecvFileInfo>;
                connected_network_->GetRecvSocket().GetList(*list);
                for (auto i : *list)
                {
                    cout << "\n" << i.name_ << " \t" << i.size_KB_;
                }

            }
            return true;
        }
        else if (!strncmp(buffer_.c_str(), "getf ", 5))
        {
            PopBuffer(5);
            std::stringstream stream(buffer_);
            std::string name;
            int index;
            stream >> name;
            stream >> index;
            connected_network_->GetFile(name, index);
            buffer_.clear();
            ResetChat();

        }
        else if (!strncmp(buffer_.c_str(), "userlist", 8))
        {
            PopBuffer(8);
            std::vector<std::string> users;
            connected_network_->GetOnlineUsers(users);
            buffer_.clear();
            cout << "\n ACTIVE USERS: \n";
            for (auto i : users)
                cout << i << '\n';
            std::cerr << "Please enter message: " << buffer_;
           
            return true;
        }
        else if (!strncmp(buffer_.c_str(), "addf ", 5))
        {
            PopBuffer(5);
            std::stringstream stream(buffer_);
            std::string pass;
            stream >> pass;
            FM_->AddFile(pass);
            buffer_.clear();
            ResetChat();

        }
        else if (!strncmp(buffer_.c_str(), "setcolor ", 9))
        {
            PopBuffer(9);
            std::string color_str("COLOR 0");
            color_str += std::to_string(atoi(&buffer_.front()));
            system(color_str.c_str());
            buffer_.clear();
            ResetChat();
            //system("COLOR 07"); white 
        }
        else if (!strncmp(buffer_.c_str(), "exit", 4))
        {
            input_is_working_ = false;
        }
        else if (!strncmp(buffer_.c_str(), "removef ", 8))
        {
            PopBuffer(8);
            std::stringstream stream(buffer_);
            int index;
            stream >> index;
            FM_->RemoveFile(index-1);
            buffer_.clear();
            ResetChat();
          
        }
        buffer_.clear();
        ResetChat();
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
	AddMsg(UserMsg{ PUBLIC, 7, "PRIVATE CHAT WITH", name });

	while (true)
	{
		ReadFromKeyboard();

		if (!strncmp(buffer_.c_str(), "/w", 2))
		{
			buffer_.clear();
			break;
		}
		UserMsg msg = { PRIVATE, msg_color_, user_name_, buffer_ };
		if (SendMsgTo(name, msg) == -1)
		{
			AddMsg(UserMsg{ PUBLIC, 7, "THERE NO USER WITH THIS NAME ", name });
			break;
		}
	}
	ResetChat();
}

unsigned ActivateChat(void* chat) //function for input thread
{
	((Chat*)chat)->InputStream();
    return 0;
}
