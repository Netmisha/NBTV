#include "AppManager.h"
#include "RecvFileInfo.h"
#include <iostream>
#include <sstream>

DLL_EXP AppManager app_man;

AppManager::~AppManager(){}

const std::vector<UserMsg> &AppManager::GetPrivateChatMsgs(const std::string &name)const
{
    return chat_.GetPrivateChatMsgs(name);
}

AppManager::AppManager()
{
    chat_.SetNetwork(&network_);
    network_.SetChat(&chat_);
    chat_.SetFM(&fm_);
    network_.SetFM(&fm_);
    network_.PrepareNetwork();
    chat_.SetUserInfo(6, "Anton");
}

void AppManager::SendFile(const std::string & path, const std::string & ip, const std::string & name)
{
    network_.SendFile(path, ip, name);
}

const std::string AppManager::GetFilePath(int file_index)const
{
    return fm_.GetFilePath(file_index);
}

const std::string AppManager::GetFileName(int file_index)const
{
    return fm_.GetFileName(file_index);
}

void AppManager::SendList(const std::string & ip)
{
    network_.SendList(ip);
}

int AppManager::SendMsg(const std::string& msg)
{
    return chat_.SendMsg(UserMsg{ PUBLIC, chat_.GetColor(), chat_.GetName(), msg });
}

int AppManager::SendMsgTo(const std::string& msg, const std::string &name)
{
    return chat_.SendMsgTo(name, UserMsg{ PRIVATE, chat_.GetColor(), chat_.GetName(), msg });
}

const std::string& AppManager::GetName()const
{
    return chat_.GetName();
}

void* AppManager::ActivateCommand(std::string& buffer) //ChangeName, On/Off private mode, filelists, get someone`s file,
                                                        //add/remove file, online users list, setcolor, exit
{
    if (buffer[0] == '/')
    {
        PopBuffer(1, buffer);

        if (!strncmp(buffer.c_str(), "w ", 2))
        {
            PopBuffer(2, buffer);
            std::stringstream stream(buffer);
            std::string name;
            stream >> name;
            PopBuffer(name.size(), buffer);
        
            UserMsg usr_msg = { PRIVATE, chat_.GetColor(), chat_.GetName(), buffer };
            if (chat_.SendMsgTo(name, usr_msg) == -1)
            {
                return new int(-1);
            }
            return NULL;
        }
        else if (!strncmp(buffer.c_str(), "setname ", 8)) //change name command
        {
            PopBuffer(8, buffer);
            if(IsNameUsed(buffer))
            {
                return NULL;
            }
            std::string old_name = chat_.GetName();
            chat_.SetName(buffer);
            network_.SendLogMsg(chat_.GetName() , LOG_UPDATE, old_name);

            return NULL;
           // ResetChat();
        }
        else if (!strncmp(buffer.c_str(), "fl ", 3))  //my and someones file list
        {
            PopBuffer(3, buffer);
            std::stringstream stream(buffer);
            std::string name;
            stream >> name;
           // ResetChat();
            if (name.empty())
            {
                std::vector<File> *list = new  std::vector<File>;
                fm_.GetFiles(*list);
                return list;
                //PrintMyList(list); //I print my list
            }
            else
            {
                network_.RequestList(name); //asking for someone`s list
                std::vector<RecvFileInfo> *list = new std::vector<RecvFileInfo>;
                network_.GetList(*list);
                return list;

            }
        }
        else if (!strncmp(buffer.c_str(), "getf ", 5)) //get file by request
        {
            PopBuffer(5, buffer);
            std::stringstream stream(buffer);
            std::string name;
            int index;
            stream >> name;
            stream >> index;
            network_.GetFile(name, index-1);
            //ResetChat();

        }
        else if (!strncmp(buffer.c_str(), "userlist", 8)) //get user list
        {
            PopBuffer(8, buffer);
            std::vector<std::string> *users = new std::vector<std::string>;
            network_.GetOnlineUsers(*users);
            return users;
        }
        else if (!strncmp(buffer.c_str(), "addf ", 5)) //add file
        {
            PopBuffer(5, buffer);
            std::stringstream stream(buffer);
            std::string pass;
            stream >> pass;
            fm_.AddFile(pass);
            //ResetChat();

        }
        else if (!strncmp(buffer.c_str(), "setcolor ", 9))
        {
            PopBuffer(9, buffer);
            std::string color_str("COLOR 0");
            color_str += std::to_string(atoi(&buffer.front()));
            system(color_str.c_str());
            //ResetChat();
            //system("COLOR 07"); white 
        }
        else if (!strncmp(buffer.c_str(), "exit", 4))
        {
           // input_is_working_ = false;
        }
        else if (!strncmp(buffer.c_str(), "removef ", 8))
        {
            PopBuffer(8, buffer);
            std::stringstream stream(buffer);
            int index;
            stream >> index;
            fm_.RemoveFile(index - 1);
            //ResetChat();

        }
       
       // ResetChat();

    }

    return false;
}

const std::string AppManager::GetIP()const
{
    return network_.GetIP();
}

void AppManager::PopBuffer(int num, std::string& buffer) //easy pop front
{
    for (int i = num; i > 0; i--)
    {
        buffer.erase(buffer.begin());
    }
}

UnpackedMessage AppManager::RecieveMessage()
{
    return network_.RecieveMessage();
}

bool AppManager::IsNameUsed(const std::string &name)const
{
    return network_.IsNameUsed(name);
}

bool AppManager::LoadUserInfo()
{
    return chat_.Load();
}