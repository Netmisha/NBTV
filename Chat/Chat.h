#pragma once

#include <vector>
#include <string>

#include "UserMsg.h"

#include "Mutex.h"
#include "Thread.h"

class Network;
class FileManager;

class Chat
{
public:
	Chat();
	~Chat();

	void SetNetwork(Network* net);
    void SetFM(FileManager* fm);

    void SetName(const std::string& name);
    const std::string& GetName(); //name getter

	void SetUserInfo(char color, const std::string& name);
	Thread& GetInputThread(); //thread descriptor getter
   

	void ResetChat() const;
	void PutMsg(const UserMsg& msg) const; // show msg to the screen
    void PrintMyList(std::vector<File>& list) const;

	void AddMsg(const UserMsg& msg); //adds msg to the vector and call`s PutMsg()
	void InputStream(); 
	void ReadFromKeyboard(); //reading from keyboard 
	void PopBuffer(int num);
	bool CheckForCommands();

	void Activate(); //starts input thread
	void ActivatePrivateChat(std::string name); //private chat input mode

    void IOfflineMsg();	//sends online msg

    void IOnlineMsg();	//sends online msg
    void IChangedName(std::string& old_name);

    void PrepareSendMsg(std::string& old_name); //instead of console input
    void PrepareAddMsg(std::string& str_msg);

    void SendMsg(const UserMsg& msg); //broadcast message and AddMsg()
    int SendMsgTo(const std::string& name, UserMsg& msg); //all msgs user write goes directly to the chosen user

    std::vector<UserMsg>& GetCurrentChat();

private:

	char msg_color_;
	std::string user_name_;
	std::string buffer_;

	std::vector< std::string > users_;
	std::vector< UserMsg > messages_;

	Network* connected_network_;
    FileManager* FM_;

	volatile bool input_is_working_;
	Thread input_thread_;
	Mutex chat_mutex_;
};

unsigned ActivateChat(void* chat); //function for input thread