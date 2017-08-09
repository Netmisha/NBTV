#ifndef NETWORK_H
#define NETWORK_H

#include "FileGetSocket.h"
#include "FileSendSocket.h"
#include "BroadcastSocket.h"
#include "RecvSocket.h"
#include "Parcer.h"
#include "FileListSendSocket.h"

#include "FileManager.h"
#include "Chat.h"
#include "SendFileInfo.h"
#include "IpNameList.h"
#include "LogMessage.h"
#include "OnlineStatus.h"

#include "Mutex.h"
#include "Thread.h"

#include <string>

class DLL Network
{
public:
    //sets up is_working_ bool and
    //sets my_ip_ as empty string
    Network();
    //calls function Cleanup
    ~Network();

    //chat uses this function to broadcast messages
    //returns size of broadcasted message in bytes
    //or -1 in case of failure
    int SendMsg(const UserMsg& user_msg)const;
    //send UserMsg to specific user determined by user_name
    //returns size of send message in bytes
    int SendMsgTo(const std::string &user_name, const UserMsg &user_msg)const;

    //function to prepare network for working
    //returns false in case of failure
    //true otherwise
    bool PrepareNetwork(unsigned int broadc_port = 0U, unsigned int tcp_port = 0U);
    //stops network
    //note that network will take some
    //time to stop, this isn't an synchronous function
    void StopNetwork();

    //sets pointer to Chat object
	void SetChat(Chat* chat);
    //sets pointer to FileManager object
    void SetFM(FileManager * fm);
    
    //send log message, name is your user name
    //type can be looked up in LogMessage.h
    //prev name is passed in case of LOG_UPDATE message
    //aka "change name case"
    int SendLogMsg(const std::string &name,
                   const LogType &type,
                   const std::string &prev_name = "");

    //get file with index from 'user_name'
	void GetFile(const std::string& user_name, int index);
    //startup thread function to GetFile function
    static unsigned GetFileStartup(void *socket_ptr);
    //sends file with specific path and name to
    //user with specific ip
    void SendFile(const std::string &path,
                  const std::string &ip,
                  const std::string &name,
                  unsigned int port = 0U);
    //thread startup function for SendFile
    static unsigned SendFileStartup(void *send_file_info);

    //request list from user with user_name
    //returns >0 if succeeds (size of send message in bytes)
    //returns 0 if user was not found
    //returns -1 if send failed
    int RequestList(const std::string& user_name);
    //send list to specific ip
    bool SendList(const std::string& ip, unsigned int port = 0U)const;
    //get list, used right after RequestList
    //return value is passed as refference parameter
    //it is safe as the sender connects to requester
    void GetList(std::vector<RecvFileInfo> &out_result)const;

    //returns vector of online users
    const std::vector<UserInfo>& GetOnlineUsers()const;
    //get your local ip
    //only valid if PrepareNetwork was called at least once
    //otherwise will return empty string
    const std::string GetIP()const;

    //recv UnpackedMessage, woun't return
    //if message can be processed by network itself
    UnpackedMessage RecieveMessage();

    //returns true if name is used by other user
    //false otherwise
    bool IsNameUsed(const std::string &name)const;

    static unsigned HeartbeatStartup(void* this_prt);
    void Heartbeat()const;

private:
    Chat *chat_;
	FileManager *FM_;

    //socket for broadcasting
    BroadcastSocket broadc_socket_;
    //socket for recieving messages
    RecvSocket recv_socket_;
    //socket for getting files
    //and list
    FileGetSocket file_get_socket_;

    //local machine ip
    //only valid if PrepareNetwork is called at least once
    std::string my_ip_;

    //bool that stops recv loop
    //if needed, just set it as 0/false
    volatile bool is_working_;

    //num of active file sharing threads
    static volatile int file_sharing_thread_num_;
    //mutex to access number of file sharing threads
    static Mutex threads_num_mutex_;

    //list of ip - user name pairs
    IpNameList ip_name_list_;
    //checks online status every 1.5 sec
    OnlineStatus online_status_check_;
    //thread with heartbeat every second
    Thread hearbeat_thread_;

    //custom tcp connection port
    unsigned int custom_tcp_port_;

    //cleanup function, closes sockets
    void Cleanup();

    //processes message from recv_str
    //returns false if message needs to be processed further (or not in network)
    //return true if message is fully processed
    //out_unp_msg is return value where unpacked version of message is stored
    //can be pathed further if needed
    bool ProcessMessage(const RecvStruct &recv_str, UnpackedMessage &out_unp_msg);

    //process log message from specific ip
    //returns true if message needs no UI representation
    //false otherwise
    bool ProcessLogMessage(const LogMessage &msg, const std::string &ip);
};

#endif // !NETWORK_H