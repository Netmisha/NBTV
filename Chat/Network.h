#ifndef NETWORK_H
#define NETWORK_H
#include "FileGetSocket.h"
#include "FileSendSocket.h"
#include "BroadcastSocket.h"
#include "RecvSocket.h"
#include "Parcer.h"

#include "FileManager.h"
#include "Chat.h"
#include "NameSearch.h"

#include "Mutex.h"

#include <map>
#include <algorithm>

class Network
{
public:
    Network();
    ~Network();

    //chat uses this function to broadcast messages
    int SendMsg(const UserMsg& user_msg);

    int SendMsgTo(const std::string &user_name, const UserMsg &user_msg);
    //function to prepare network for working
    bool PrepareNetwork();
    //thread func for starting network
    static unsigned StartNetwork(void *network_ptr);
    //function that loops recv and processes messages
    //to stop - call StopNetwork
    void LoopRecv();
    //stops network
    void StopNetwork();

	void SetChat(Chat* chat);
    void SetFM(FileManager * fm);
    

    void SendLogMsg(const std::string &name, const LogType &type);

	void GetFile(const std::string& user_name, int index);
	void SendFile(const std::string& pass, const std::string& ip, std::string& name);

    void RequestSomeoneList(const std::string& name);
    void SendList(const std::string& ip);

    void GetOnlineUsers(std::vector<std::string> &users);

    
private:
    Chat *chat_;
	FileManager *FM_;
    //socket for broadcasting
    BroadcastSocket broadc_socket_;
    //socket for recieving messages
    RecvSocket recv_socket_;

    //socket for getting files
    FileGetSocket file_get_socket_;

    std::string my_ip_;

    //bool that stops recv loop
    //if needed, just set it as 0/false
    volatile bool is_working_;
    //mutex on send
    Mutex send_mutex_;

    std::map<std::string, std::string> user_ip_name_map_;

    //cleanup function, closes sockets
    void Cleanup();

    void ProcessMessage(const RecvStruct &recv_str);

    void ProcessLogMessage(const LogMessage &msg, const std::string &ip);
};

#endif // !NETWORK_H