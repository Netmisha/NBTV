#include "AppManager.h"
#include "RecvFileInfo.h"
#include <iostream>
#include <sstream>

void AppManager::Work()
{
    network_.PrepareNetwork();
    Thread network_loop(Network::StartNetwork, &network_);

    chat_.SetUserInfo(6, "Anton");
    chat_.Activate();
	
    chat_.GetInputThread().Join();

    network_.StopNetwork();
    chat_.IOfflineMsg();
    network_loop.Join();


	std::cout << "ok";
	//at the end
}


AppManager::~AppManager()
{

}

AppManager::AppManager()
{
    chat_.SetNetwork(&network_);
    network_.SetChat(&chat_);
    chat_.SetFM(&fm_);
    network_.SetFM(&fm_);
}

void* AppManager::ActivateCommand(std::string buffer)
{
    if (buffer[0] == '/')
    {
        PopBuffer(1, buffer);

        if (!strncmp(buffer.c_str(), "w ", 2))
        {
            PopBuffer(2, buffer);
           // ActivatePrivateChat(buffer);
            return NULL;
        }
        else if (!strncmp(buffer.c_str(), "setname ", 8))
        {
            PopBuffer(8, buffer);
            std::string old_name = chat_.GetName();
            chat_.SetName(buffer);
            chat_.IChangedName(old_name);
            return NULL;
           // ResetChat();
        }
        else if (!strncmp(buffer.c_str(), "fl ", 3))
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
                network_.RequestSomeoneList(name); //asking for someone`s list
                std::vector<RecvFileInfo> *list = new std::vector<RecvFileInfo>;
                network_.GetRecvSocket().GetList(*list);
                return list;

            }
        }
        else if (!strncmp(buffer.c_str(), "getf ", 5))
        {
            PopBuffer(5, buffer);
            std::stringstream stream(buffer);
            std::string name;
            int index;
            stream >> name;
            stream >> index;
            network_.GetFile(name, index);
            //ResetChat();

        }
        else if (!strncmp(buffer.c_str(), "userlist", 8))
        {
            PopBuffer(8, buffer);
            std::vector<std::string> *users = new std::vector<std::string>;
            network_.GetOnlineUsers(*users);
            return users;
        }
        else if (!strncmp(buffer.c_str(), "addf ", 5))
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

void AppManager::PopBuffer(int num, std::string& buffer) //easy pop front
{
    for (int i = num; i > 0; i--)
    {
        buffer.erase(buffer.begin());
    }
}