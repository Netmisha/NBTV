#include "FileManager.h"

#include <Windows.h>

FileManager::FileManager()
{
    Load();
}

FileManager::~FileManager()
{
    Save();
}

bool FileManager::AddFile(const std::string &path)
{
    File new_file;
    if(!new_file.SetFile(path))
    {
        return false;
    }

    shared_files_.push_back(new_file);
    return true;
}

void FileManager::RemoveFile(int file_index)
{
    if(file_index < shared_files_.size())
        shared_files_.erase(shared_files_.begin() + file_index);
}

std::string FileManager::GetFilePath(int file_index)const
{
    if(file_index < shared_files_.size())
        return shared_files_[file_index].GetPath();

    return std::string("");
}

std::string FileManager::GetFileName(int file_index)const
{
    if(file_index < shared_files_.size())
        return shared_files_[file_index].GetName();

    return std::string("");
}

const double FileManager::GetFileSizeKB(int file_index)const
{
    if(file_index < shared_files_.size())
        return shared_files_[file_index].GetSizeKB();

    return 0;
}

const double FileManager::GetFileSizeMB(int file_index)const
{
    if(file_index < shared_files_.size())
        return shared_files_[file_index].GetSizeMB();

    return 0;
}

void FileManager::GetFilePaths(std::vector<std::string> &out_result)const
{
    for(File it : shared_files_)
    {
        out_result.push_back(it.GetPath());
    }
}

void FileManager::GetFileNames(std::vector<std::string> &out_result)const
{
    for(File it : shared_files_)
    {
        out_result.push_back(it.GetName());
    }
}

void FileManager::GetFiles(std::vector<File> &out_result)const
{
    for(File file : shared_files_)
    {
        out_result.push_back(file);
    }
}

void FileManager::Save()
{
    if((CreateDirectory(FILE_DATA_SAVE_DIR, NULL)) ||
       (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        HANDLE file = CreateFile(FILE_DATA_SAVE_FULLPATH,   //path
                                 GENERIC_WRITE,             //to write
                                 0,                         //non-share
                                 NULL,                      //security
                                 CREATE_ALWAYS,             //always create
                                 FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                                 NULL);                     //why no default arguments
        if(file == INVALID_HANDLE_VALUE)
        {
            return;
        }
        int bytes_written;
        std::vector<std::string> file_paths;
        GetFilePaths(file_paths);

        for(std::string path : file_paths)
        {
            int path_size = path.length();
            WriteFile(file,
                      &path_size,
                      sizeof(path_size),
                      (DWORD*)&bytes_written,
                      NULL);

            WriteFile(file,
                      &path[0],
                      path_size,
                      (DWORD*)&bytes_written,
                      NULL);
        }
        CloseHandle(file);
    }
}

void FileManager::Load()
{
    HANDLE file = CreateFile(FILE_DATA_SAVE_FULLPATH,   //path
                             GENERIC_READ,              //to read
                             0,                         //non-share
                             NULL,                      //security
                             OPEN_EXISTING,             //only existing
                             FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                             NULL);
    if(file == INVALID_HANDLE_VALUE)
    {
        return;
    }
    
    DWORD bytes_read = 0;
    File shared_file;
    std::string path;
    while(true)
    {
        int path_size;
        BOOL error_flag = ReadFile(file,
                                   &path_size,
                                   sizeof(path_size),
                                   &bytes_read,
                                   NULL);
        if(error_flag && bytes_read < (int)sizeof(path_size))
        {
            //eof
            break;
        }

        path.resize(path_size);

        error_flag = ReadFile(file,
                              &path[0],
                              path_size,
                              &bytes_read,
                              NULL);

        if(shared_file.SetFile(path))
            shared_files_.push_back(shared_file);
    }

    CloseHandle(file);
}
