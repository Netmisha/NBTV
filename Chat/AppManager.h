#pragma once

#include "Network.h"
#include "Chat.h"
#include "FileManager.h"
#include <vector>
class AppManager
{
	Chat chat_;
	Network network_;
    FileManager fm_;

public:
	void Work();
	AppManager();
	~AppManager();



    //function to use from UI (.DLL)
    
    std::vector<UserMsg> GetCurrentChat();

   // void GetMsgLoop(); //message getting

    void SendFile(const std::string &path, const std::string &ip, const std::string &name);

    std::string GetFilePath(int i);
    std::string GetFileName(int i);

    void ProcessLogMessage(const LogMessage &msg, const std::string &ip);
    void SendList(const std::string& ip);

    void SendMsg(std::string& msg); //after user wrote message in chat
    void AddMsg(UserMsg& ms); //will be used by recv loop

    void* ActivateCommand(std::string& msg); //called by buttons with different commands
                                             //ChangeName, On/Off private mode, filelists, get someone`s file,
                                             //add/remove file, online users list, setcolor, exit
    std::string GetIP();
    void PopBuffer(int num, std::string& buffer); //easy pop front
 
    void ActivatePrivateChat();
    void ExitPrivateChat();

    void EXIT();
};

