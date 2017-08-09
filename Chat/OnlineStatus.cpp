#include "OnlineStatus.h"

OnlineStatus::OnlineStatus() : ip_name_list_(NULL), is_working_(false) {}

OnlineStatus::~OnlineStatus() {}

void OnlineStatus::SetIpNameList(IpNameList* list)
{
    ip_name_list_ = list;
}

void OnlineStatus::Add(const std::string &ip)
{
    online_list_access_mutex_.Lock();
    online_list_[ip] = true;
    online_list_access_mutex_.Unlock();
}

void OnlineStatus::IpOnline(const std::string &ip)
{
    Add(ip);
}

void OnlineStatus::Start()
{
    check_thread_.BeginThread(OnlineStatus::CheckStartup, this);
}

void OnlineStatus::Stop()
{
    is_working_ = false;
    check_thread_.Join();
}

unsigned OnlineStatus::CheckStartup(void *this_ptr)
{
    (*(OnlineStatus*)this_ptr).OfflineCheck();
    return 0;
}

void OnlineStatus::OfflineCheck()
{
    is_working_ = true;

    while(is_working_)
    {
        Sleep(1500);

        online_list_access_mutex_.Lock();
        for(std::pair<std::string, bool> pair : online_list_)
        {
            //if online status is false
            if(!pair.second)
            {
                ip_name_list_->Remove(pair.first);
                online_list_.erase(pair.first);
            }
            //if it is true
            else
            {
                pair.second = false;
            }
        }
        online_list_access_mutex_.Unlock();
    }
}