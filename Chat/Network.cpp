#include "Network.h"

#include <stdlib.h>

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

	if (!file_send_socket_.Initialize())
	{
		return false;
	}
    if (!file_send_socket_.Initialize())
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

int Network::SendMsg(UserMsg user_msg)
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
        
        ProcessMessage(packet);
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

void Network::SendLogMsg(const std::string &name, const LogType &type)
{
    LogMessage log_msg = { type, name };
    void *send_buffer = NULL;
    int send_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, send_buffer);

    broadc_socket_.Send(send_buffer, send_size); //err_check
}

void Network::GetFile(std::string user_name, int index)
{
	int file_index;
	void *send_buffer = NULL;
	int send_size = Parcer::PackMessage(GET_FILE_MESSAGE, &file_index, send_buffer);

	//search for ip
	std::map<std::string, std::string>::iterator it = std::find_if(user_ip_name_map_.begin(),
		user_ip_name_map_.end(),
		NameSearch(&user_name));

	broadc_socket_.SendTo(send_buffer, send_size, it->second.c_str());
    file_get_socket_.GetFile(std::to_string(index));
}

void Network::SendFile(std::string pass, std::string ip)
{
	 file_send_socket_.SendFile(pass, ip);
}

void Network::ProcessLogMessage(const LogMessage &msg, const std::string &ip)
{
    switch(msg.type_)
    {
    case LOG_ONLINE:
        {
            LogMessage log_msg = { LOG_RESPONCE, chat_->GetName() };
            void *answ_log_msg = NULL;
            int msg_size = Parcer::PackMessage(LOG_MESSAGE, &log_msg, answ_log_msg);
            broadc_socket_.SendTo(answ_log_msg, msg_size, ip.c_str());
        }
        //do not put break here
    case LOG_RESPONCE:
        user_ip_name_map_[ip] = msg.name_;
        break;

    case LOG_OFFLINE:
        user_ip_name_map_.erase(ip);
        break;
    }
}

int Network::SendMsgTo(const std::string &user_name, const UserMsg &user_msg)
{
    std::map<std::string, std::string>::iterator it = std::find_if(user_ip_name_map_.begin(),
                                                                   user_ip_name_map_.end(),
                                                                   NameSearch(&user_name));
    if(it == user_ip_name_map_.end())
    {
        return -1;
    }

    void *packet = NULL;
    int packet_size = Parcer::PackMessage(CHAT_MESSAGE, (void*)&user_msg, packet); //allocation in heap

    send_mutex_.Lock();
    packet_size = broadc_socket_.SendTo(packet, packet_size, it->second.c_str());
    send_mutex_.Unlock();

    delete[] packet;
    return packet_size;
}

void Network::ProcessMessage(const RecvStruct &recv_str)
{
    if(recv_str.ip_ != my_ip_ || BROADCAST_LOOPBACK)
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
				SendFile( FM_.GetFileName( *((int*)unp_msg.msg_) ), recv_str.ip_);
				break;
        }

        delete unp_msg.msg_;
    }

}