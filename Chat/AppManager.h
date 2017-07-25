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
    std::vector<std::string> GetUserList();
    std::vector<File> GetFileList();

    void GetMsgLoop(); //message getting

    void SendMsg(std::string msg); //after user wrote message in chat
    void PutMsg(); //will be used by recv loop

    void* ActivateCommand(std::string msg); //called by buttons with different commands
                                     //example: ChangeMsgColor, ChangeName, ChangeHomeDir, DownloadSomeone`sFile
                                     //AddFile, RemoveFile
    void PopBuffer(int num, std::string& buffer); //easy pop front
 
    void ActivatePrivateChat();
    void ExitPrivateChat();
    void EXIT();
};

