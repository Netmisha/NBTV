#ifndef FIND_PAIR_BY_IP_H
#define FIND_PAIR_BY_IP_H

#include <string>
#include <utility>

class FindPairByIp
{
public:
    FindPairByIp(const std::string &to_find);
    bool operator()(const std::pair<std::string, bool> &pair)const;
private:
    const std::string &ip_to_find_;
};

#endif // !FIND_PAIR_BY_IP_H