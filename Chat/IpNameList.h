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

    //adds pair or changes user name part of pair
    //if pair with same ip part already exists
    void Add(const std::string &ip, const std::string &name);

    //removes pair with specific ip
    void Remove(const std::string &ip);

    //get name by ip
    //if there is none - returns empty string
    std::string GetName(const std::string &ip)const;
    //get ip by name
    //if there is none - returns empty string
    std::string GetIp(const std::string &name)const;

    //used to get vector of all user names
    //return value is passed as refference parameter
    void GetNameList(std::vector<std::string> &out_result)const;

    //returns true if name is currently used
    //false otherwise
    bool IsNameUsed(const std::string &name)const;

private:
    //map of ip - username pairs
    std::map<std::string, std::string> ip_name_map_;
};
#endif // !IP_NAME_LIST_H