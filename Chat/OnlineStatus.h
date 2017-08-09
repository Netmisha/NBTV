#ifndef ONLINE_STATUS_H
#define ONLINE_STATUS_H

#include <map>
#include <string>

#include "IpNameList.h"
#include "Mutex.h"
#include "Thread.h"

class OnlineStatus
{
public:

    OnlineStatus();
    ~OnlineStatus();

    void SetIpNameList(IpNameList* list);
    void Add(const std::string &ip);
    void IpOnline(const std::string &ip);

    void Start();
    void Stop();
    
    static unsigned CheckStartup(void *this_ptr);
    void OfflineCheck();

private:
    std::map<std::string, bool> online_list_;
    Mutex online_list_access_mutex_;

    IpNameList* ip_name_list_;

    volatile bool is_working_;
    Thread check_thread_;

};

#endif // !ONLINE_STATUS_H