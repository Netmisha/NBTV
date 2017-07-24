#ifndef IP_NAME_LIST_H
#define IP_NAME_LIST_H

#include "NameSearch.h"

#include <map>
#include <string>
#include <vector>

class IpNameList
{
public:
    IpNameList();
    ~IpNameList();

    void Add(const std::string &ip, const std::string &name);
    void Remove(const std::string &ip);
    std::string GetName(const std::string &ip)const;
    std::string GetIp(const std::string &name)const;
    void GetNameList(std::vector<std::string> &out_result);

private:

    std::map<std::string, std::string> ip_name_map_;
};
#endif // !IP_NAME_LIST_H