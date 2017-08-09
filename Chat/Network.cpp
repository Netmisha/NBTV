#include "Network.h"

#include <stdlib.h>
#include <algorithm>

volatile int Network::file_sharing_thread_num_ = 0;
Mutex Network::threads_num_mutex_;

Network::Network() : is_working_(false), my_ip_(""), custom_tcp_port_(0U){}

Network::~Network()
{
    Cleanup();
}

bool Network::PrepareNetwork(unsigned int broadc_port, unsigned int tcp_port)
{
    WSAData wsa_data;
    int error_check = WSAStartup(MAKEWORD(WSA_MIN_VERSION, WSA_MAX_VERSION), 
                                &wsa_data);
    if(error_check != 0)
    {
        return false;
    }

    if(!broadc_socket_.Initialize(broadc_port))
    {
        return false;
    }

    if(!recv_socket_.Initialize(broadc_port))
    {
        return false;
    }

    if (!file_get_socket_.Initialize(tcp_port))
	{
		return false;
	}

    if(tcp_port)
        custom_tcp_port_ = tcp_port;

    //getting local pc ip
    int rand_value = rand(), buffer = 0;
    void *rand_value_send = NULL;
    //allocation in heap
    buffer = Parcer::PackMessage(PREPARE_MESSAGE, &rand_value, rand_value_send);
    error_check = broadc_socket_.Send(rand_value_send, buffer);
    delete[] rand_value_send;
    if(error_check == SOCKET_ERROR)
    {
        return false;
    }
    
    int err_check;
    while(true)
    {
        RecvStruct recv_struct;
        err_check = recv_socket_.Recv(recv_struct);

        if(err_check == SOCKET_ERROR)
            return false;

        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_struct.packet_);
        recv_struct.Clear();

        if((unp_msg.type_ == PREPARE_MESSAGE) &&
           (*(int*)unp_msg.msg_ == rand_value))
        {
            my_ip_ = recv_struct.ip_;
            delete[] unp_msg.msg_;
            break;
        }

        unp_msg.Clear();
    }
    //-----------------------
    online_status_check_.SetIpNameList(&ip_name_list_);
    online_status_check_.Start();

    hearbeat_thread_.BeginThread(Network::HeartbeatStartup, this);

    is_working_ = true;
    return true;
}

int Network::SendMsg(const UserMsg& user_msg)const
{
    void *packet = NULL;
    //allocation in heap
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, &user_msg, packet);

    packet_size = broadc_socket_.Send(packet, packet_size);

    delete[] packet; //clearing heap
    return packet_size;
}

void Network::Cleanup()
{
    WSACleanup();
}

void Network::StopNetwork()
{
    is_working_ = false;
    online_status_check_.Stop();
    hearbeat_thread_.Join();
}

void Network::SetChat(Chat * chat)
{
	chat_ = chat;
}

void Network::SetFM(FileManager * fm)
{
    FM_ = fm;
}

int Network::SendLogMsg(const std::string &name,
                        const LogType &type,
                        const std::string &prev_name)
{
    LogMessage log_msg = { type, name, prev_name, chat_->GetColor() };
    void *send_buffer = NULL;
    //allocation in heap
    int send_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, send_buffer);

    send_size = broadc_socket_.Send(send_buffer, send_size);
    delete[] send_buffer;
    return send_size;
}

void Network::GetFile(const std::string& user_name, int index)
{
	short file_index = (short)index;
	void *send_buffer = NULL;
    //allocation in heap
	int send_size = Parcer::PackMessage(GET_FILE_MESSAGE, &file_index, send_buffer);

    broadc_socket_.SendTo(send_buffer,
                          send_size,
                          ip_name_list_.GetIp(user_name).c_str());

    delete[] send_buffer;

    Thread(FileGetSocket::GetFileStartup, &file_get_socket_);
}


int Network::RequestList(const std::string& user_name)
{
    std::string user_ip = ip_name_list_.GetIp(user_name);
    if(user_ip.empty())
    {
        return 0;
    }

    void *send_buffer = NULL;
    //allocation in heap
    int send_size = Parcer::PackMessage(FILE_LIST_REQUEST, NULL, send_buffer);
    
    
    send_size = broadc_socket_.SendTo(send_buffer,
                                      send_size,
                                      user_ip.c_str());
    delete[] send_buffer;
    return send_size;
}

bool Network::SendList(const std::string& ip, unsigned int port)const
{
    const std::vector<File> files = FM_->GetFiles();

    return FileListSendSocket().SendFileList(files, ip, port);
}

bool Network::ProcessLogMessage(const LogMessage &msg, const std::string &ip)
{
    bool is_fully_processed = false;
    switch(msg.type_)
    {
    case LOG_ONLINE:
        {
            LogMessage log_msg = { LOG_RESPONCE, chat_->GetName() };
            void *answ_log_msg = NULL;
            //allocation in heap
            int msg_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, answ_log_msg);
            broadc_socket_.SendTo(answ_log_msg, msg_size, ip.c_str());
            delete[] answ_log_msg;
        }
        //do not put break here
    
    case LOG_UPDATE:
        if(!msg.prev_name_.empty())
            chat_->ChangeOtherUserName(msg.prev_name_, msg.name_);

    case LOG_RESPONCE:
        ip_name_list_.Add(ip, msg.name_, msg.color_);
        
        if(msg.type_ == LOG_RESPONCE)
            is_fully_processed = true;

        break;

    case LOG_OFFLINE:
        ip_name_list_.Remove(ip);
        break;
    }

    return is_fully_processed;
}

int Network::SendMsgTo(const std::string &user_name, const UserMsg &user_msg)const
{
    std::string ip = ip_name_list_.GetIp(user_name);
    if(ip.empty())
    {
        return -1;
    }

    void *packet = NULL;
    //allocation in heap
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, (void*)&user_msg, packet);

    packet_size = broadc_socket_.SendTo(packet, packet_size, ip.c_str());

    delete[] packet;
    return packet_size;
}

bool Network::ProcessMessage(const RecvStruct &recv_str, UnpackedMessage &out_unp_msg)
{
    bool is_fully_processed = (is_working_ ? true : false);

    if((recv_str.ip_ != my_ip_) || BROADCAST_LOOPBACK)
    {
        //allocation in heap
        out_unp_msg = Parcer::UnpackMessage(recv_str.packet_);

        switch(out_unp_msg.type_)
        {

        case CHAT_MESSAGE:
            chat_->AddMsg(*(UserMsg*)out_unp_msg.msg_);
            is_fully_processed = false;
            break;

        case LOG_MESSAGE:
            is_fully_processed = ProcessLogMessage(*(LogMessage*)out_unp_msg.msg_,
                                                   recv_str.ip_);
            break;

        case GET_FILE_MESSAGE:
            SendFile(FM_->GetFilePath(*((short*)out_unp_msg.msg_)),
                     recv_str.ip_,
                     FM_->GetFileName(*((short*)out_unp_msg.msg_)),
                     custom_tcp_port_);
            break;

        case FILE_LIST_REQUEST:
            SendList(recv_str.ip_);
            break;

        case HEARTBEAT_MESSAGE:
            online_status_check_.IpOnline(recv_str.ip_);
            break;

        default:
            is_fully_processed = false;
            break;
        }
    }

    return is_fully_processed;
}

const std::vector<UserInfo>& Network::GetOnlineUsers()const
{
    return ip_name_list_.GetNameList();
}

const std::string Network::GetIP()const
{
    return my_ip_;
}

unsigned Network::SendFileStartup(void *send_file_info)
{
    SendFileInfo* file_info = (SendFileInfo*)send_file_info;
    threads_num_mutex_.Lock();
    file_sharing_thread_num_++;
    threads_num_mutex_.Unlock();

    FileSendSocket().SendFile(file_info->path_,
                              file_info->ip_,
                              file_info->name_,
                              file_info->port_);
    delete send_file_info;

    threads_num_mutex_.Lock();
    file_sharing_thread_num_--;
    threads_num_mutex_.Unlock();

    return 0;
}

void Network::SendFile(const std::string &path,
                       const std::string &ip,
                       const std::string &name,
                       unsigned int port)
{
    //explisit conversion to object
    //then copying it to heap
    SendFileInfo *sfi = new SendFileInfo(SendFileInfo{ path, name, ip, port });
    Thread th(Network::SendFileStartup, sfi);
}

unsigned Network::GetFileStartup(void *params)
{
    threads_num_mutex_.Lock();
    file_sharing_thread_num_++;
    threads_num_mutex_.Unlock();

    ((FileGetSocket*)params)->GetFile();

    threads_num_mutex_.Lock();
    file_sharing_thread_num_--;
    threads_num_mutex_.Unlock();

    delete[] params;
    return 0;
}

UnpackedMessage Network::RecieveMessage()
{
    RecvStruct packet;
    UnpackedMessage unp_msg;

    while(is_working_)
    {
        unp_msg.type_ = INVALID_TYPE;
        recv_socket_.Recv(packet);
        bool is_fully_processed = ProcessMessage(packet, unp_msg);
        packet.Clear();
        if(is_fully_processed)
        {
            if(unp_msg.type_ != INVALID_TYPE)
                unp_msg.Clear();

            continue;
        }
        break;
    }
    return unp_msg;
}

void Network::GetList(std::vector<RecvFileInfo> &out_result)const
{
    file_get_socket_.GetList(out_result);
}

bool Network::IsNameUsed(const std::string &name)const
{
    return ip_name_list_.IsNameUsed(name);
}

unsigned Network::HeartbeatStartup(void* this_prt)
{
    (*(Network*)this_prt).Heartbeat();
    return 0;
}

void Network::Heartbeat()const
{
    void *message;
    int message_size = Parcer::PackMessage(HEARTBEAT_MESSAGE, NULL, message);
    while(is_working_)
    {
        Sleep(4000);

        broadc_socket_.Send(message, message_size);
    }

    delete[] message;
}