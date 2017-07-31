#ifndef FILE_H
#define FILE_H

#include "Defines.h"
#include <string>

class DLL_EXP File
{
public:
    File();
    File(const std::string &path);
    ~File();

    //true if path is value
    //false otherwise
    bool SetFile(const std::string &path);
    bool IsValid()const;

    const std::string GetName()const;
    const std::string GetPath()const;
    const double GetSizeKB()const;
    const double GetSizeMB()const;

private:
    std::string name_,
                path_;
    double size_KB_;
};

#endif // !FILE_STRUCT_H