#ifndef NETWORK_H
#define NETWORK_H

#include "BroadcastSocket.h"
#include "RecvSocket.h"
#include "UserMsg.h"
#include "Parcer.h"
#include "Chat.h"

#include <thread>   //change to API
#include <mutex>    //same
#include <map>

class Network
{
public:
    Network();
    ~Network();

    //chat uses this function to broadcast messages
    int SendMsg(UserMsg user_msg);

    int SendMsgTo(const std::string &user_name, const UserMsg &user_msg);
    //function to prepare network for working
    bool PrepareNetwork();
    //thread func for starting network
    static void StartNetwork(void *network_ptr);
    //function that loops recv and processes messages
    //to stop - call StopNetwork
    void LoopRecv();
    //stops network
    void StopNetwork();

	void SetChat(Chat* chat);

    void SendLogMsg(const std::string &name, const LogType &type);
    
private:
    Chat *chat_;
    //socket for broadcasting
    BroadcastSocket broadc_socket_;
    //socket for recieving messages
    RecvSocket recv_socket_;

    std::string my_ip_;

    //bool that stops recv loop
    //if needed, just set it as 0/false
    volatile bool is_working_;
    //mutex on send
    std::mutex send_mutex_;

    std::map<std::string, std::string> users_name_ip_map_;

    //cleanup function, closes sockets
    void Cleanup();

    void ProcessMessage(const RecvStruct &recv_str);

    void ProcessLogMessage(const LogMessage &msg, const std::string &ip);
};

#endif // !NETWORK_H