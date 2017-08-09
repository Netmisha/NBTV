#include "OnlineStatus.h"

#include "FindPairByIp.h"

OnlineStatus::OnlineStatus() : ip_name_list_(NULL), is_working_(false) {}

OnlineStatus::~OnlineStatus() {}

void OnlineStatus::SetIpNameList(IpNameList* list)
{
    ip_name_list_ = list;
}

void OnlineStatus::Add(const std::string &ip)
{
    if(std::find_if(online_list_.begin(), online_list_.end(), FindPairByIp(ip)) == online_list_.end())
    {
        online_list_.push_back(std::pair<std::string, bool>(ip, false));
    }
}

void OnlineStatus::IpOnline(const std::string &ip)
{
    std::vector<std::pair<std::string, bool>>::iterator it = std::find_if(online_list_.begin(),
                                                                          online_list_.end(),
                                                                          FindPairByIp(ip));
    if(it != online_list_.end())
    {
        it->second = true;
    }
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

    std::vector<std::pair<std::string, bool>>::iterator it;

    while(is_working_)
    {
        Sleep(ONLINE_CHECK_INTERVAL_MSEC);

        online_list_access_mutex_.Lock();
        for(it = online_list_.begin(); it != online_list_.end();)
        {
            if(it->second == false)
            {
                ip_name_list_->Remove(it->first);
                online_list_.erase(it);
            }
            else
            {
                it->second = false;
                ++it;
            }
        }
        online_list_access_mutex_.Unlock();
    }
}