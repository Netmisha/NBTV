#include "NameSearch.h"

NameSearch::NameSearch(){}

NameSearch::NameSearch(const std::string *name_to_search) : name_to_search_(name_to_search) {}

NameSearch::~NameSearch(){}

void NameSearch::SetNameToSearch(const std::string *name_to_search)
{
    name_to_search_ = name_to_search;
}

bool NameSearch::operator()(const std::pair<std::string, UserInfo> &ip_name_pair)const
{
    return ((ip_name_pair.second.user_name_) == *name_to_search_);
}