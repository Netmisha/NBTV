#include "File.h"

#include <sstream>

#include "Defines.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <Windows.h>

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

    char path_delim = '\\';

    if(std::find(path.begin(), path.end(), path_delim) == path.end())
        path_delim = '/';

    std::stringstream str_stream;

    str_stream << path_;
    while(!str_stream.eof())
    {
        //shortening path to file name
        std::getline(str_stream, name_, path_delim);
    }
    
    HANDLE file = CreateFile(path_.c_str(),             //path
                             GENERIC_READ,              //to read
                             0,                         //non-share
                             NULL,                      //security
                             OPEN_EXISTING,             //only existing
                             FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                             NULL);
    if(file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    //bytes divided by kilobyte size
    size_KB_ = GetFileSize(file, NULL) / (double)WINDOWS_KILOBYTE;

    CloseHandle(file);
    return true;
}

bool File::IsValid()const
{
    return (bool)PathFileExistsA(path_.c_str());
}

const std::string& File::GetName()const
{
    return name_;
}

const std::string& File::GetPath()const
{
    return path_;
}

const double File::GetSizeKB()const
{
    return size_KB_;
}

const double File::GetSizeMB()const
{
    return size_KB_ / WINDOWS_KILOBYTE;
}