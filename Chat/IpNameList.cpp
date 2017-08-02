#include "IpNameList.h"

#include <algorithm>
#include "Defines.h"

IpNameList::IpNameList(){}

IpNameList::~IpNameList(){}

void IpNameList::Add(const std::string &ip, const std::string &name)
{
    ip_name_map_[ip] = name;
}

void IpNameList::Remove(const std::string &ip)
{
    ip_name_map_.erase(ip);
}

std::string IpNameList::GetName(const std::string &ip)const
{
    return ip_name_map_.find(ip)->second;
}

std::string IpNameList::GetIp(const std::string &name)const
{
    std::map<std::string, std::string>::const_iterator it = find_if(ip_name_map_.begin(),
                                                                    ip_name_map_.end(),
                                                                    NameSearch(&name));
    if(it != ip_name_map_.cend())
        return it->first;
    return std::string("");
}

void IpNameList::GetNameList(std::vector<std::string> &out_result)const
{
    for(std::pair<std::string, std::string> pair_ : ip_name_map_)
    {
        out_result.push_back(pair_.second);
    }
}

bool IpNameList::IsNameUsed(const std::string &name)const
{
    return !(GetIp(name).empty()) || (name == PUBLIC_MSGS);
}