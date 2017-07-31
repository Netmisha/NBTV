#include "Network.h"

#include <stdlib.h>
#include <algorithm>

volatile int Network::file_sharing_thread_num_ = 0;
Mutex Network::threads_num_mutex_;

Network::Network() : is_working_(false){}

Network::~Network()
{
    Cleanup();
}

bool Network::PrepareNetwork()
{
    WSAData wsa_data;
    int error_check = WSAStartup(MAKEWORD(WSA_MIN_VERSION, WSA_MAX_VERSION), 
                                &wsa_data);
    if(error_check != 0)
    {
        return false;
    }

    if(!broadc_socket_.Initialize())
    {
        return false;
    }

    if(!recv_socket_.Initialize())
    {
        return false;
    }

    if (!file_get_socket_.Initialize())
	{
		return false;
	}

    //getting local pc ip
    int rand_value = rand(), buffer = 0;
    void *rand_value_send = NULL;
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
        err_check = recv_socket_.Recv(&recv_struct);

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
    is_working_ = true;
    return true;
}

int Network::SendMsg(const UserMsg& user_msg)const
{
    void *packet = NULL;
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, &user_msg, packet); //allocation in heap

    send_mutex_.Lock();
    packet_size = broadc_socket_.Send(packet, packet_size);
    send_mutex_.Unlock();

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
    LogMessage log_msg = { type, name, prev_name };
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, send_buffer);

    send_size = broadc_socket_.Send(send_buffer, send_size);
    delete[] send_buffer;
    return send_size;
}

void Network::GetFile(const std::string& user_name, int index)
{
	int file_index = index;
	void *send_buffer = NULL;
	int send_size = Parcer::PackMessage(GET_FILE_MESSAGE, &file_index, send_buffer);

    broadc_socket_.SendTo(send_buffer,
                          send_size,
                          ip_name_list_.GetIp(user_name).c_str());

    delete[] send_buffer;

    Thread(FileGetSocket::GetFileStartup, &file_get_socket_);
}


int Network::RequestList(const std::string& user_name)
{
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(FILE_LIST_REQUEST, NULL, send_buffer);
    
    send_size = broadc_socket_.SendTo(send_buffer,
                                      send_size,
                                      ip_name_list_.GetIp(user_name).c_str());
    delete[] send_buffer;
    return send_size;
}

void Network::SendList(const std::string& ip)const
{
    std::vector<File> files;
    FM_->GetFiles(files);

    FileListSendSocket().SendFileList(files, ip);
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
            int msg_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, answ_log_msg);
            broadc_socket_.SendTo(answ_log_msg, msg_size, ip.c_str());
            delete[] answ_log_msg;
        }
        //do not put break here
    
    case LOG_UPDATE:
    case LOG_RESPONCE:
        ip_name_list_.Add(ip, msg.name_);
        
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

    send_mutex_.Lock();
    packet_size = broadc_socket_.SendTo(packet, packet_size, ip.c_str());
    send_mutex_.Unlock();

    delete[] packet;
    return packet_size;
}

bool Network::ProcessMessage(const RecvStruct &recv_str, UnpackedMessage &out_unp_msg)
{
    bool is_fully_processed = true;

    if((recv_str.ip_ != my_ip_) || BROADCAST_LOOPBACK)
    {
        //allocation in heap
        out_unp_msg = Parcer::UnpackMessage(recv_str.packet_);

        switch(out_unp_msg.type_)
        {

        case LOG_MESSAGE:
            is_fully_processed = ProcessLogMessage(*(LogMessage*)out_unp_msg.msg_,
                                                   recv_str.ip_);
            break;

        case GET_FILE_MESSAGE:
            SendFile(FM_->GetFilePath(*((int*)out_unp_msg.msg_)),
                     recv_str.ip_,
                     FM_->GetFileName(*((int*)out_unp_msg.msg_)));
            break;

        case FILE_LIST_REQUEST:
            SendList(recv_str.ip_);
            break;

        default:
            is_fully_processed = false;
            break;
        }
    }

    return is_fully_processed;
}

void Network::GetOnlineUsers(std::vector<std::string> &out_users)const
{
    ip_name_list_.GetNameList(out_users);
}

const std::string Network::GetIP()const
{
    return my_ip_;
}

unsigned Network::SendFileStartup(void *send_file_info)
{
    threads_num_mutex_.Lock();
    file_sharing_thread_num_++;
    threads_num_mutex_.Unlock();

    FileSendSocket().SendFile((*(SendFileInfo*)send_file_info).path_,
                              (*(SendFileInfo*)send_file_info).ip_,
                              (*(SendFileInfo*)send_file_info).name_);
    delete send_file_info;

    threads_num_mutex_.Lock();
    file_sharing_thread_num_--;
    threads_num_mutex_.Unlock();

    return 0;
}

void Network::SendFile(const std::string &path,
                       const std::string &ip,
                       const std::string &name)
{
    //explisit conversion to object
    //then copying it to heap
    SendFileInfo *sfi = new SendFileInfo(SendFileInfo{ path, name, ip });
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
        recv_socket_.Recv(&packet);
        bool is_fully_processed = ProcessMessage(packet, unp_msg);
        packet.Clear();
        if(is_fully_processed)
        {
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