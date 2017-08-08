#ifndef MESSAGES_ARCHIVE_H
#define MESSAGES_ARCHIVE_H

#include "UserMsg.h"
#include "Defines.h"
#include "Mutex.h"

#include <map>
#include <vector>

class MessagesArchive
{
public:
    MessagesArchive();
    ~MessagesArchive();

    //add message to archive
    void AddMsg(const UserMsg &msg, const std::string &name);
    //get pointer to vector with msgs from private chat with 'user_name'
    std::vector<UserMsg>* GetUserMsgs(const std::string &user_name)const;
    //change user name in map
    void ChangeUsername(const std::string &from, const std::string &to);

private:
    
    std::map<std::string, std::vector<UserMsg>*> messages_;
    //mutex for changing map
    Mutex add_msg_mutex_;
    //free allocated memory and clear messages_ map
    void ClearMessages();
};

#endif // !MESSAGES_ARCHIVE_H