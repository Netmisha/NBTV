#include "Network.h"
#include <iostream>

Network::Network() : is_working_(false){}

Network::~Network()
{
    Cleanup();
}

int Network::PrepareNetwork()
{
    WSAData wsa_data;
    int error_check = WSAStartup(WSA_VERSION, &wsa_data);
    if(error_check != 0)
    {
        return error_check;
    }

    error_check = broadc_socket_.Initialize();
    if(error_check != 0)
    {
        return error_check;
    }

    error_check = recv_socket_.Initialize();
    if(error_check != 0)
    {
        return error_check;
    }

    int rand_value = rand(), buffer = 0;
    void *rand_value_send;
    buffer = Parcer::PackMessage(PREPARE_MESSAGE, &rand_value, rand_value_send);
    error_check = broadc_socket_.Send(rand_value_send, buffer);
    if(error_check == SOCKET_ERROR)
    {
        return error_check;
    }
    
    while(true)
    {
        RecvStruct recv_struct;
        int j = recv_socket_.Recv(&recv_struct);

        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_struct.packet_);
        if(unp_msg.type_ == PREPARE_MESSAGE ||
           *(int*)unp_msg.msg_ == rand_value)
        {
            my_ip_ = recv_struct.ip_;
            delete[] unp_msg.msg_;
            break;
        }

        delete[] unp_msg.msg_;
    }

    return 0;
}

int Network::SendMsg(UserMsg user_msg)
{
    void *packet = NULL;
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, &user_msg, packet); //allocation in heap

    send_mutex_.lock();
    packet_size = broadc_socket_.Send(packet, packet_size);
    send_mutex_.unlock();

    delete[] packet; //clearing heap
    return packet_size;
}

void Network::Cleanup()
{
    broadc_socket_.Close();
    recv_socket_.Close();
    WSACleanup();
}

void Network::StartNetwork(void *network_ptr)
{
    ((Network*)network_ptr)->LoopRecv();
}

void Network::LoopRecv()
{
    is_working_ = true;
    while(is_working_)
    {
        RecvStruct packet;
        recv_socket_.Recv(&packet);
        
        if(packet.ip_ != my_ip_ || BROADCAST_LOOPBACK)
        {
            //allocation in heap
            UnpackedMessage unp_msg = Parcer::UnpackMessage(packet.packet_);
            
            switch(unp_msg.type_)
            {
                case CHAT_MESSAGE:
                    chat_->AddMsg(*(UserMsg*)unp_msg.msg_);
                    break;
                case LOG_MESSAGE:
                    ProcessLogMessage(*(LogMessage*)unp_msg.msg_, packet.ip_);
                    break;
            }

            delete unp_msg.msg_;
        }
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

void Network::SendLogMsg(std::string name, int type)
{
    LogMessage log_msg = { type, name };
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, send_buffer);

    broadc_socket_.Send(send_buffer, send_size); //err_check
}

void Network::ProcessLogMessage(LogMessage msg, std::string ip)
{
    if(msg.type_ == LOG_ONLINE)
    {
        if(users_name_ip_map_.find(msg.name_) != users_name_ip_map_.end())
        {
            users_name_ip_map_.erase(msg.name_);
        }
        users_name_ip_map_[msg.name_] = ip;
    }
    else
    {
        users_name_ip_map_.erase(msg.name_);
    }
}

int Network::SendMsgTo(std::string user_name, UserMsg &user_msg)
{
    std::map<std::string, std::string>::iterator it = users_name_ip_map_.find(user_name);
    if(it == users_name_ip_map_.end())
    {
        return -1;
    }

    void *packet = NULL;
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, &user_msg, packet);

    send_mutex_.lock();
    packet_size = broadc_socket_.SendTo(packet, packet_size, it->second.c_str());
    send_mutex_.unlock();

    delete[] packet;
    return packet_size;
}