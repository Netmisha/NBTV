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
    if(error_check == SOCKET_ERROR)
    {
        return false;
    }
    
    while(true)
    {
        RecvStruct recv_struct;
        recv_socket_.Recv(&recv_struct);

        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_struct.packet_);
        if((unp_msg.type_ == PREPARE_MESSAGE) &&
           (*(int*)unp_msg.msg_ == rand_value))
        {
            my_ip_ = recv_struct.ip_;
            delete[] unp_msg.msg_;
            break;
        }

        delete[] unp_msg.msg_;
    }
    //-----------------------

    return true;
}

int Network::SendMsg(const UserMsg& user_msg)
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
    broadc_socket_.Close();
    recv_socket_.Close();
    file_get_socket_.Close();
    WSACleanup();
}

unsigned Network::StartNetwork(void *network_ptr)
{
    ((Network*)network_ptr)->LoopRecv();
    return 0;
}

void Network::LoopRecv()
{
    is_working_ = true;
    while(is_working_)
    {
        RecvStruct packet;
        recv_socket_.Recv(&packet);
        
        ProcessMessage(packet);
    }

    while(file_sharing_thread_num_)
    {
        //give process time to another thread
        Sleep(1);
    }
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

void Network::SendLogMsg(const std::string &name, const LogType &type)
{
    LogMessage log_msg = { type, name };
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, send_buffer);

    broadc_socket_.Send(send_buffer, send_size); //err_check
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


void Network::RequestSomeoneList(const std::string& name)
{
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(FILE_LIST_REQUEST, NULL, send_buffer);
    
    broadc_socket_.SendTo(send_buffer, send_size, ip_name_list_.GetIp(name).c_str()); //err_check
}

void Network::SendList(const std::string& ip)
{
    std::vector<std::string> files;
    FM_->GetFileNames(files);
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(FILE_LIST_MESSAGE, &files, send_buffer);


    broadc_socket_.SendTo(send_buffer, send_size, ip.c_str()); //err_check
}



void Network::ProcessLogMessage(const LogMessage &msg, const std::string &ip)
{
    switch(msg.type_)
    {
    case LOG_ONLINE:
        {
            LogMessage log_msg = { LOG_UPDATE, chat_->GetName() };
            void *answ_log_msg = NULL;
            int msg_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, answ_log_msg);
            broadc_socket_.SendTo(answ_log_msg, msg_size, ip.c_str());
        }
        //do not put break here
    case LOG_UPDATE:;
        ip_name_list_.Add(ip, msg.name_);
        break;

    case LOG_OFFLINE:
        ip_name_list_.Remove(ip);
        break;
    }
}

int Network::SendMsgTo(const std::string &user_name, const UserMsg &user_msg)
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

void Network::ProcessMessage(const RecvStruct &recv_str)
{
    if((recv_str.ip_ != my_ip_) || BROADCAST_LOOPBACK)
    {
        //allocation in heap
        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_str.packet_);

        switch(unp_msg.type_)
        {
        case CHAT_MESSAGE:
            chat_->AddMsg(*(UserMsg*)unp_msg.msg_);
            break;

        case LOG_MESSAGE:
            ProcessLogMessage(*(LogMessage*)unp_msg.msg_, recv_str.ip_);
            break;

        case GET_FILE_MESSAGE:
            SendFile(FM_->GetFilePath(*((int*)unp_msg.msg_)),
                     recv_str.ip_,
                     FM_->GetFileName(*((int*)unp_msg.msg_)));
            break;

        case FILE_LIST_REQUEST:
            SendList(recv_str.ip_);
            break;

        case FILE_LIST_MESSAGE:
            chat_->PrintSomeoneList(*((std::vector<std::string>*)unp_msg.msg_));
            break;
        }

        delete unp_msg.msg_;
    }

}

void Network::GetOnlineUsers(std::vector<std::string> &out_users)
{
    ip_name_list_.GetNameList(out_users);
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

Mutex& Network::GetSharingNumMutex()
{
    return threads_num_mutex_;
}

volatile int& Network::GetSharingThreadsNum()
{
    return file_sharing_thread_num_;
}