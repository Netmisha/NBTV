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
    error_check = broadc_socket_.Send(&rand_value, sizeof(rand_value));
    if(error_check == SOCKET_ERROR)
    {
        return error_check;
    }
    
    while(true)
    {
        RecvStruct recv_struct;
        recv_socket_.Recv(&recv_struct);
        if((int)*recv_struct.packet_ == rand_value)
        {
            my_ip_ = recv_struct.ip_;
            break;
        }
    }

    return 0;
}

int Network::SendMsg(UserMsg user_msg)
{
    void *packet = NULL;
    int packet_size = Parcer::PackMessage(user_msg, packet); //allocation in heap

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
            UserMsg processed_msg = Parcer::UnpackMessage(packet.packet_);
            chat_->AddMsg(processed_msg);
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
