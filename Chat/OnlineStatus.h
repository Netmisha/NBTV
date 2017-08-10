#ifndef ONLINE_STATUS_H
#define ONLINE_STATUS_H

#include <map>
#include <string>
#include <algorithm>

#include "IpNameList.h"
#include "Mutex.h"
#include "Thread.h"

class OnlineStatus
{
public:

    OnlineStatus();
    ~OnlineStatus();

    //sets ptr to IpNameList object
    void SetIpNameList(IpNameList* list);
    //adds ip to checks
    void Add(const std::string &ip);
    //sets ip online status as true for next check
    void IpOnline(const std::string &ip);
    //remove ip from checks
    void Remove(const std::string &ip);

    //starts online checks
    void Start();
    //stops online checks and joins thread
    void Stop();
    
    //startup thread function for OfflineCheck
    static unsigned CheckStartup(void *this_ptr);
    //loop that performs checks
    void OfflineCheck();
    //true if update needed
    //false otherwise
    bool CheckForUpdate();

private:
    //vector of ip - online status pairs to be checked
    std::vector<std::pair<std::string, bool>> online_list_;
    //mutex to change vector
    Mutex online_list_access_mutex_;
    
    IpNameList* ip_name_list_;

    //thread stopping bool
    volatile bool is_working_;
    //thread for online checks
    Thread check_thread_;
    //bool to check if ui need update
    volatile bool is_update_needed_;

    void CheckList();
};

#endif // !ONLINE_STATUS_H