#include "Parcer.h"

#include "File.h"
#include "UserMsg.h"
#include "LogMessage.h"
#include "RecvFileInfo.h"

#include <iostream>
#include <vector>
#include <string>

namespace Parcer
{
    int  PackMessage(const MessageType &type, const void *in_msg, void* &out_result)
    {
        int packet_size = -1;
        switch(type)
        {
        case PREPARE_MESSAGE:
            packet_size = PackPrepMessage(in_msg, out_result);
            break;
        case LOG_MESSAGE:
            packet_size = PackLogMessage(in_msg, out_result);
            break;
        case CHAT_MESSAGE:
            packet_size = PackChatMessage(in_msg, out_result);
            break;
        case GET_FILE_MESSAGE:
            packet_size = PackGetFileMessage(in_msg, out_result);
            break;
        case FILE_LIST_REQUEST:
            packet_size = PackFileListRequest(in_msg, out_result);
            break;
        case FILE_LIST_MESSAGE:
            packet_size = PackFileList(in_msg, out_result);
            break;
        case HEARTBEAT_MESSAGE:
            packet_size = PackHeartbeat(in_msg, out_result);
            break;
        }

        return packet_size;
    }

    UnpackedMessage UnpackMessage(const void *packet)
    {
        UnpackedMessage result;
        if(!packet)
        {
            return result;
        }
        char *temp_ptr = (char*)packet;
        result.type_ = (MessageType)*temp_ptr++;
        switch(result.type_)
        {
        case PREPARE_MESSAGE:
            result.msg_ = ParcePrepMessage(temp_ptr);
            break;
        case LOG_MESSAGE:
            result.msg_ = ParceLogMessage(temp_ptr);
            break;
        case CHAT_MESSAGE:
            result.msg_ = ParceChatMessage(temp_ptr);
            break;
        case GET_FILE_MESSAGE:
            result.msg_ = ParceGetFileMessage(temp_ptr);
            break;
        case FILE_LIST_REQUEST:
            break;
        case FILE_LIST_MESSAGE:
            result.msg_ = ParceFileList(temp_ptr);
            break;
        case HEARTBEAT_MESSAGE:
            break;
        default:
            result.type_ = INVALID_TYPE;
            break;
        }

        return result;
    }

    int  PackChatMessage(const void *in_msg, void* &out_packet)
    {
        UserMsg *user_msg = (UserMsg*)in_msg;
        int pack_size = CHAT_MESSAGE_HEADER_SIZE + user_msg->name_.length() + user_msg->msg_.length();
        out_packet = new char[pack_size]();

        unsigned char *temp_ptr = (unsigned char*)out_packet;
        *temp_ptr++ = (unsigned char)CHAT_MESSAGE;
        *temp_ptr++ = (unsigned char)user_msg->type_;
        *temp_ptr++ = (unsigned char)user_msg->name_.length();
        *temp_ptr++ = user_msg->color_;
        *temp_ptr++ = (unsigned char)user_msg->msg_.length();

        memcpy(temp_ptr,
               &user_msg->name_[0],
               user_msg->name_.length());

        memcpy(temp_ptr + user_msg->name_.length(),
               &user_msg->msg_[0],
               user_msg->msg_.length());

        return pack_size;
    }

    void*  ParceChatMessage(const void *in_packet)
    {
        UserMsg *result = new UserMsg();
        unsigned char name_size, msg_size;
        unsigned char *temp_ptr = (unsigned char*)in_packet;
        result->type_ = (ChatMsgType)*temp_ptr++;
        name_size = *temp_ptr++;
        result->color_ = *temp_ptr++;
        msg_size = *temp_ptr++;

        result->name_.resize(name_size);
        memcpy(&result->name_[0], temp_ptr, name_size);

        result->msg_.resize(msg_size);
        memcpy(&result->msg_[0], temp_ptr + name_size, msg_size);

        return (void*)result;
    }

    int  PackPrepMessage(const void *in_msg, void* &out_packet)
    {
        out_packet = new char[PREPARE_MESSAGE_SIZE]();
        unsigned char* temp_ptr = (unsigned char*)out_packet;
        *temp_ptr = PREPARE_MESSAGE;
        *(int*)(temp_ptr + 1) = *(int*)in_msg;

        return PREPARE_MESSAGE_SIZE;
    }

    void*  ParcePrepMessage(const void *in_packet)
    {
        int* result = new int();
        *result = *(int*)in_packet;
        return (void*)result;
    }

    int  PackLogMessage(const void *in_msg, void* &out_packet)
    {
        LogMessage *log_msg = (LogMessage*)in_msg;
        int msg_size = log_msg->name_.length()
            + log_msg->prev_name_.length()
            + LOG_MESSAGE_HEADER_SIZE;
        out_packet = new char[msg_size]();

        unsigned char *temp_ptr = (unsigned char*)out_packet;
        *temp_ptr++ = LOG_MESSAGE;
        *temp_ptr++ = log_msg->type_;
        *temp_ptr++ = (unsigned char)log_msg->name_.length();
        *temp_ptr++ = log_msg->color_;
        *temp_ptr++ = (unsigned char)log_msg->prev_name_.length();

        memcpy(temp_ptr, &log_msg->name_[0], log_msg->name_.length());
        temp_ptr += log_msg->name_.length();
        memcpy(temp_ptr, &log_msg->prev_name_[0], log_msg->prev_name_.length());

        return msg_size;
    }

    void* ParceLogMessage(const void *in_packet)
    {
        LogMessage *result = new LogMessage();
        unsigned char *temp_ptr = (unsigned char*)in_packet;

        result->type_ = (LogType)*temp_ptr++;
        int name_size = (int)*temp_ptr++;
        result->color_ = *temp_ptr++;
        int prev_name_size = (int)*temp_ptr++;

        result->name_.resize(name_size);
        memcpy(&result->name_[0], temp_ptr, name_size);
        temp_ptr += name_size;

        result->prev_name_.resize(prev_name_size);
        memcpy(&result->prev_name_[0], temp_ptr, prev_name_size);

        return (void*)result;
    }

    int  PackGetFileMessage(const void * in_msg, void *& out_packet)
    {
        int msg_size = GET_FILE_MESSAGE_SIZE;
        out_packet = new char[msg_size]();

        unsigned char *temp_ptr = (unsigned char*)out_packet;
        *temp_ptr++ = GET_FILE_MESSAGE;
        *(short*)temp_ptr++ = *((short*)in_msg);

        return msg_size;
    }

    void* ParceGetFileMessage(const void * in_packet)
    {
        short *result = new short;
        *result = *((short*)in_packet);

        return (void*)result;
    }


    int  PackFileListRequest(const void *in_msg, void* &out_packet)
    {
        //[1] for delete[] not to be UB
        out_packet = new char[1]();
        *(unsigned char*)out_packet = FILE_LIST_REQUEST;
        return FILE_LIST_REQUEST_SIZE;
    }

    int  PackFileList(const void *in_msg, void* &out_packet)
    {
        File *file_info = (File*)in_msg;
        std::string name = file_info->GetName();

        out_packet = new char[FILE_LIST_MESSAGE_SIZE]();
        unsigned char *temp_ptr = (unsigned char*)out_packet;

        *temp_ptr++ = FILE_LIST_MESSAGE;

        *(double*)temp_ptr = file_info->GetSizeKB();
        temp_ptr += sizeof(double);

        *(short*)temp_ptr = name.length();
        temp_ptr += sizeof(short);

        memcpy(temp_ptr, &name[0], name.length());

        return FILE_LIST_MESSAGE_SIZE;
    }

    void*  ParceFileList(const void *in_packet)
    {
        RecvFileInfo *recv_file_info = new RecvFileInfo();
        char *temp_ptr = (char*)in_packet;

        recv_file_info->size_KB_ = *(double*)temp_ptr;
        temp_ptr += sizeof(double);

        short name_size = *(short*)temp_ptr;
        recv_file_info->name_.resize(name_size);
        memcpy(&recv_file_info->name_[0], temp_ptr + sizeof(short), name_size);

        return recv_file_info;
    }

    int PackHeartbeat(const void *in_msg, void* &out_packet)
    {
        out_packet = new char[1]();
        *(unsigned char*)out_packet = HEARTBEAT_MESSAGE;
        return HEARTBEAT_MESSAGE_SIZE;
    }
}