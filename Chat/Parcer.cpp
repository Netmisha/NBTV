#include "Parcer.h"
#include <iostream>

int Parcer::PackMessage(const UserMsg &user_msg, void* &out_result)
{
    int pack_size = 3 + user_msg.name_.length() + user_msg.msg_.length();
    out_result = new char[pack_size]();

    char *temp_ptr = (char*)out_result;
    *temp_ptr++ = (char)user_msg.name_.length();
    *temp_ptr++ = user_msg.color_;
    *temp_ptr++ = (char)user_msg.msg_.length();

    memcpy(temp_ptr,
           &user_msg.name_[0],
           user_msg.name_.length());

    memcpy(temp_ptr + user_msg.name_.length(),
           &user_msg.msg_[0],
           user_msg.msg_.length());

    return pack_size;
}

UserMsg Parcer::UnpackMessage(void *packet)
{
    UserMsg result;
    unsigned char name_size, msg_size;
    char *temp_ptr = (char*)packet;
    name_size = (unsigned char)*temp_ptr++;
    result.color_ = (unsigned char)*(temp_ptr++);
    msg_size = (unsigned char)*(temp_ptr++);

    result.name_.resize(name_size);
    memcpy(&result.name_[0], temp_ptr, name_size);

    result.msg_.resize(msg_size);
    memcpy(&result.msg_[0], temp_ptr + name_size, msg_size);

    return result;
}