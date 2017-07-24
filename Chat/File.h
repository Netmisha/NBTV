#ifndef FILE_H
#define FILE_H

#include <string>

class File
{
public:
    File();
    File(const std::string &path);
    ~File();

    //true if path is value
    //false otherwise
    bool SetFile(const std::string &path);
    bool IsValid();

    std::string GetName()const;
    std::string GetPath()const;

private:
    std::string name_,
                path_;
};

#endif // !FILE_STRUCT_H