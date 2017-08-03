#include "IpNameList.h"

#include <algorithm>
#include "Defines.h"

IpNameList::IpNameList(){}

IpNameList::~IpNameList(){}

void IpNameList::Add(const std::string &ip, const std::string &name, char color)
{
    if(ip_name_map_.find(ip) != ip_name_map_.end())
    {
        Remove(ip);
    }

    ip_name_map_[ip] = UserInfo{ name, color };
    online_users_.push_back(UserInfo{ name, color });
}

void IpNameList::Remove(const std::string &ip)
{
    online_users_.erase(std::find(online_users_.begin(),
                                  online_users_.end(),
                                  ip_name_map_[ip]));
    ip_name_map_.erase(ip);
}

std::string IpNameList::GetName(const std::string &ip)const
{
    return ip_name_map_.find(ip)->second.user_name_;
}

std::string IpNameList::GetIp(const std::string &name)const
{
    std::map<std::string, UserInfo>::const_iterator it = find_if(ip_name_map_.begin(),
                                                                    ip_name_map_.end(),
                                                                    NameSearch(&name));
    if(it != ip_name_map_.cend())
        return it->first;
    return std::string("");
}

const std::vector<UserInfo>& IpNameList::GetNameList()const
{
    return online_users_;
}

bool IpNameList::IsNameUsed(const std::string &name)const
{
    return !(GetIp(name).empty()) || (name == PUBLIC_MSGS);
}