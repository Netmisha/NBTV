#include "Parcer.h"
#include <iostream>

int Parcer::PackMessage(int type, void *in_msg, void* &out_result)
{
    int packet_size = -1;
    switch(type)
    {
        case PREPARE_MESSAGE:
            packet_size = Parcer::PackPrepMessage(in_msg, out_result);
            break;
        case LOG_MESSAGE:
            packet_size = Parcer::PackLogMessage(in_msg, out_result);
            break;
        case CHAT_MESSAGE:
            packet_size = Parcer::PackChatMessage(in_msg, out_result);
            break;
    }

    return packet_size;
}

UnpackedMessage Parcer::UnpackMessage(void *packet)
{
    UnpackedMessage result;
    char *temp_ptr = (char*)packet;
    result.type_ = *temp_ptr++;
    switch(result.type_)
    {
        case PREPARE_MESSAGE:
            result.msg_ = Parcer::ParcePrepMessage(temp_ptr);
            break;
        case LOG_MESSAGE:
            result.msg_ = Parcer::ParceLogMessage(temp_ptr);
            break;
        case CHAT_MESSAGE:
            result.msg_ = Parcer::ParceChatMessage(temp_ptr);
            break;
        default:
            result.type_ = INVALID_TYPE;
            break;
    }

    return result;
}

int Parcer::PackChatMessage(void *in_msg, void* &out_packet)
{
    UserMsg *user_msg = (UserMsg*)in_msg;
    int pack_size = CHAT_MESSAGE_HEADER_SIZE + user_msg->name_.length() + user_msg->msg_.length();
    out_packet = new char[pack_size]();

    char *temp_ptr = (char*)out_packet;
    *temp_ptr++ = (unsigned char)CHAT_MESSAGE;
    *temp_ptr++ = (char)user_msg->name_.length();
    *temp_ptr++ = user_msg->color_;
    *temp_ptr++ = (char)user_msg->msg_.length();

    memcpy(temp_ptr,
           &user_msg->name_[0],
           user_msg->name_.length());

    memcpy(temp_ptr + user_msg->name_.length(),
           &user_msg->msg_[0],
           user_msg->msg_.length());

    return pack_size;
}

void* Parcer::ParceChatMessage(void *in_packet)
{
    UserMsg *result = new UserMsg();
    unsigned char name_size, msg_size;
    char *temp_ptr = (char*)in_packet;
    name_size = (unsigned char)*temp_ptr++;
    result->color_ = (unsigned char)*(temp_ptr++);
    msg_size = (unsigned char)*(temp_ptr++);

    result->name_.resize(name_size);
    memcpy(&result->name_[0], temp_ptr, name_size);

    result->msg_.resize(msg_size);
    memcpy(&result->msg_[0], temp_ptr + name_size, msg_size);

    return (void*)result;
}

int Parcer::PackPrepMessage(void *in_msg, void* &out_packet)
{
    out_packet = new char[PREPARE_MESSAGE_SIZE]();
    char* temp_ptr = (char*)out_packet;
    *temp_ptr = PREPARE_MESSAGE;
    *(int*)(temp_ptr + 1) = *(int*)in_msg;
    
    return PREPARE_MESSAGE_SIZE;
}

void* Parcer::ParcePrepMessage(void *in_packet)
{
    int* result = new int();
    *result = *(int*)in_packet;
    return (void*)result;
}

int Parcer::PackLogMessage(void *in_msg, void* &out_packet)
{
    LogMessage *log_msg = (LogMessage*)in_msg;
    int msg_size = log_msg->name_.length() + LOG_MESSAGE_HEADER_SIZE;
    out_packet = new char[msg_size]();
    
    char *temp_ptr = (char*)out_packet;
    *temp_ptr++ = log_msg->type_;
    *temp_ptr++ = (unsigned char)log_msg->name_.length();
    memcpy(temp_ptr, &log_msg->name_[0], log_msg->name_.length());

    return msg_size;
}

void* Parcer::ParceLogMessage(void *in_packet)
{
    LogMessage *result = new LogMessage();
    char *temp_ptr = (char*)in_packet;

    result->type_ = *temp_ptr++;
    int name_size = *temp_ptr++;
    result->name_.resize(name_size);

    memcpy(&result->name_[0], temp_ptr, name_size);
    
    return (void*)result;
}