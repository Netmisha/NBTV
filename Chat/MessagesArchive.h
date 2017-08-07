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
    std::vector<UserMsg>* GetUserMsgs(const std::string &user_name)const;
    void ChangeUsername(const std::string &from, const std::string &to) {}

private:

    std::map<std::string, std::vector<UserMsg>*> messages_;

    Mutex add_msg_mutex_;

    void ClearMessages();
};

#endif // !MESSAGES_ARCHIVE_H