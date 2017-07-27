#ifndef NAME_SEARCH_H
#define NAME_SEARCH_H

#include <string>
#include <utility>  //std::pair

class NameSearch
{
public:
    NameSearch();
    NameSearch(const std::string *name_to_search);
    ~NameSearch();

    void SetNameToSearch(const std::string *name_to_search);
    bool operator()(const std::pair<std::string, std::string> &ip_name_pair)const;

private:
    const std::string *name_to_search_;
};

#endif // !NAME_SEARCH_H
