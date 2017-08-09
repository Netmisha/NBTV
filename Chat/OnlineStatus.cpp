#include "OnlineStatus.h"

OnlineStatus::OnlineStatus() : ip_name_list_(NULL), is_working_(false) 
{
    online_list_.clear();
}

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

    std::vector<std::string> online_ips_;
    std::map<std::string, bool>::iterator it;

    while(is_working_)
    {
        Sleep(5000);

        ip_name_list_->GetIpVector(online_ips_);

        online_list_access_mutex_.Lock();
        for(std::string ip : online_ips_)
        {
            it = online_list_.find(ip);
            if(it == online_list_.end())
                continue;
            else if(it->second == false)
            {
                ip_name_list_->Remove(ip);
            }
            else
            {
                online_list_[ip] = false;
            }
        }
        online_list_access_mutex_.Unlock();

        online_ips_.clear();
    }
}