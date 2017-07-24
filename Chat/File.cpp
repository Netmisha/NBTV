#include "File.h"

File::File() {}

File::File(const std::string &path)
{
    SetFile(path);
}

File::~File() {}

bool File::SetFile(const std::string &path)
{
    path_ = path;
    if(!IsValid())
    {
        return false;
    }

    std::stringstream str_stream;

    str_stream << path_;
    while(!str_stream.eof())
    {
        //shortening path to file name
        std::getline(str_stream, name_, '\\');
    }

    return true;
}

bool File::IsValid()
{
    return (std::ifstream(path_).good());
}

std::string File::GetName()const
{
    return name_;
}

std::string File::GetPath()const
{
    return path_;
}