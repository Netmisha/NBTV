#include "OnlineStatus.h"

#include "FindPairByIp.h"

OnlineStatus::OnlineStatus() : ip_name_list_(NULL),
                               is_working_(false),
                               is_update_needed_(false) {}

OnlineStatus::~OnlineStatus() {}

void OnlineStatus::SetIpNameList(IpNameList* list)
{
    ip_name_list_ = list;
}

void OnlineStatus::Add(const std::string &ip)
{
    online_list_access_mutex_.Lock();
    if(std::find_if(online_list_.begin(), online_list_.end(), FindPairByIp(ip)) == online_list_.end())
    {
        online_list_.push_back(std::pair<std::string, bool>(ip, false));
    }
    online_list_access_mutex_.Unlock();
}

void OnlineStatus::IpOnline(const std::string &ip)
{
    online_list_access_mutex_.Lock();
    std::vector<std::pair<std::string, bool>>::iterator it = std::find_if(online_list_.begin(),
                                                                          online_list_.end(),
                                                                          FindPairByIp(ip));
    if(it != online_list_.end())
    {
        it->second = true;
    }
    online_list_access_mutex_.Unlock();
}

void OnlineStatus::Remove(const std::string &ip)
{
    online_list_access_mutex_.Lock();
    std::vector<std::pair<std::string, bool>>::iterator it = std::find_if(online_list_.begin(),
                                                                          online_list_.end(),
                                                                          FindPairByIp(ip));
    if(it != online_list_.end())
        online_list_.erase(it);

    online_list_access_mutex_.Unlock();
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

    
    unsigned int timer_id = SetTimer(NULL,                      //no attached windows
                                     NULL,                      //no attached event
                                     ONLINE_CHECK_INTERVAL_MSEC,   //interval
                                     (TIMERPROC)NULL);          //no attached function
    MSG msg;
    while(is_working_)
    {
        //if there is a message, get it and remove it from queue
        if(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //if message type is correct
            if((msg.message == WM_TIMER) && (msg.hwnd == NULL) && (msg.wParam == timer_id))
            {
                //check online list
                CheckList();
            }
        }

        Sleep(0);
    }
    KillTimer(NULL, timer_id);
}

void OnlineStatus::CheckList()
{
    static std::vector<std::pair<std::string, bool>>::iterator it;

    online_list_access_mutex_.Lock();
    for(it = online_list_.begin(); it != online_list_.end();)
    {
        if(it->second == false)
        {
            ip_name_list_->Remove(it->first);
            online_list_.erase(it);
            is_update_needed_ = true;
        }
        else
        {
            it->second = false;
            ++it;
        }
    }
    online_list_access_mutex_.Unlock();
}

bool OnlineStatus::CheckForUpdate()
{
    bool result = is_update_needed_;

    if(result)
        is_update_needed_ = false;

    return result;
}