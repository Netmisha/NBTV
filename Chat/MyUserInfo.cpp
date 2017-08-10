#include "MyUserInfo.h"

#include <Windows.h>
#include "Defines.h"

MyUserInfo::MyUserInfo() : my_user_info_(UserInfo{ "", 0 }) 
{}

MyUserInfo::~MyUserInfo() 
{
    Save();
}

const std::string& MyUserInfo::GetName()const
{
    return my_user_info_.user_name_;
}

void MyUserInfo::SetName(const std::string &name)
{
    my_user_info_.user_name_ = name;
}

char MyUserInfo::GetColor()const
{
    return my_user_info_.color_;
}

void MyUserInfo::SetColor(char color)
{
    my_user_info_.color_ = color;
}

bool MyUserInfo::Load()
{
    HANDLE file = CreateFileA(USER_DATA_SAVE_FULLPATH,   //path
                              GENERIC_READ,              //to read
                              0,                         //non-share
                              NULL,                      //security
                              OPEN_EXISTING,             //only existing
                              FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                              NULL);
    //if there is no existing file
    if(file == INVALID_HANDLE_VALUE)
        return false;

    BOOL error_flag;
    DWORD bytes_read = 0;
    char buffer[BUFFER_SIZE] = { 0 };
    error_flag = ReadFile(file,
                          &buffer,
                          BUFFER_SIZE,
                          &bytes_read,
                          NULL);

    if(!error_flag)
    {
        my_user_info_.color_ = DEFAULT_COLOR;
        my_user_info_.user_name_ = "";
        CloseHandle(file);
        return false;
    }
    
    my_user_info_.color_ = buffer[0];
    my_user_info_.user_name_.resize((unsigned char)buffer[1]);
    memcpy(&my_user_info_.user_name_[0], buffer + 2, (int)buffer[1]);

    CloseHandle(file);
    return true;
}

void MyUserInfo::Save()const
{
    //if directory to save shared files data
    //doesn't exist -> create it
    if((CreateDirectory(DATA_SAVE_DIR, NULL)) ||
       (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        HANDLE file = CreateFileA(USER_DATA_SAVE_FULLPATH,   //path
                                  GENERIC_WRITE,             //to write
                                  0,                         //non-share
                                  NULL,                      //security
                                  CREATE_ALWAYS,             //always create
                                  FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                                  NULL);                     //why no default arguments
        //if failed to create file
        if(file == INVALID_HANDLE_VALUE)
        {
            return;
        }

        BOOL err_check;
        do
        {
            int bytes_written;
            err_check = WriteFile(file,
                                  &my_user_info_.color_,
                                  sizeof(my_user_info_.color_),
                                  (DWORD*)&bytes_written,
                                  NULL);

            if(!err_check)
                break;

            unsigned char size = (unsigned char)my_user_info_.user_name_.length();
            err_check = WriteFile(file,
                                  &size,
                                  sizeof(size),
                                  (DWORD*)&bytes_written,
                                  NULL);

            if(!err_check)
                break;

            err_check = WriteFile(file,
                                  &my_user_info_.user_name_[0],
                                  size,
                                  (DWORD*)&bytes_written,
                                  NULL);

            int i = 0;
        } while(false);

        CloseHandle(file);

        //if writing failed - delete file
        //as it is corrupted
        if(!err_check)
            DeleteFileA(USER_DATA_SAVE_FULLPATH);
    }
}