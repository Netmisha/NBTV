#include "MessagesArchive.h"
#include "Defines.h"

MessagesArchive::MessagesArchive() {}

MessagesArchive::~MessagesArchive() 
{
    ClearMessages();
}

void MessagesArchive::AddMsg(const UserMsg &msg, const std::string &name)
{
    std::string msg_chat =
        (msg.type_ == PUBLIC ? PUBLIC_MSGS : (name.empty() ? msg.name_ : name));

    add_msg_mutex_.Lock();
    //if vector of user messages to this user do not exist
    if(messages_.find(msg_chat) == messages_.end())
        messages_[msg_chat] = new std::vector<UserMsg>;

    std::vector<UserMsg> *msgs = messages_[msg_chat];
    msgs->push_back(msg);

    if((int)msgs->size() > (msg.type_ == PUBLIC ? 
        MAX_PUBLIC_MSGS_STORED : MAX_PRIVATE_MSGS_STORED))
    {
        msgs->erase(msgs->begin());
    }

    add_msg_mutex_.Unlock();
}

std::vector<UserMsg>* MessagesArchive::GetUserMsgs(const std::string &user_name)const
{
    std::map<std::string, std::vector<UserMsg>*>::const_iterator it = messages_.find(user_name);

    if(it == messages_.end())
        return NULL;

    return it->second;
}

void MessagesArchive::ClearMessages()
{
    for(std::pair<std::string, std::vector<UserMsg>*> pair : messages_)
    {
        delete pair.second;
    }
    messages_.clear();
}