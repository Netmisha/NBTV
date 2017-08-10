#include "FindPairByIp.h"

FindPairByIp::FindPairByIp(const std::string &to_find) : ip_to_find_(to_find) {}

bool FindPairByIp::operator()(const std::pair<std::string, bool> &pair)const
{
    return pair.first == ip_to_find_;
}