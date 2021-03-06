#include "FileManager.h"

#include <Windows.h>

FileManager::FileManager() :error_string_("")
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
    if((unsigned)file_index < shared_files_.size())
        shared_files_.erase(shared_files_.begin() + file_index);
}

const std::string& FileManager::GetFilePath(int file_index)const
{
    if((unsigned)file_index < shared_files_.size())
        return shared_files_[file_index].GetPath();

    return error_string_;
}

const std::string& FileManager::GetFileName(int file_index)const
{
    if((unsigned)file_index < shared_files_.size())
        return shared_files_[file_index].GetName();

    return error_string_;
}

const double FileManager::GetFileSizeKB(int file_index)const
{
    if((unsigned)file_index < shared_files_.size())
        return shared_files_[file_index].GetSizeKB();

    return 0;
}

const double FileManager::GetFileSizeMB(int file_index)const
{
    if((unsigned)file_index < shared_files_.size())
        return shared_files_[file_index].GetSizeMB();

    return 0;
}

bool FileManager::GetFile(int file_index, File &out_result)const
{
    if((unsigned)file_index < shared_files_.size())
    {
        out_result = shared_files_[file_index];
        return true;
    }
    return false;
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

const std::vector<File>& FileManager::GetFiles()const
{
    return shared_files_;
}

void FileManager::Save()const
{
    //if directory to save shared files data
    //doesn't exist -> create it
    if((CreateDirectory(DATA_SAVE_DIR, NULL)) ||
       (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        HANDLE file = CreateFile(FILE_DATA_SAVE_FULLPATH,   //path
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

        int bytes_written;
        std::vector<std::string> file_paths;
        GetFilePaths(file_paths);
        BOOL err_check = FALSE;
        for(std::string path : file_paths)
        {
            int path_size = path.length();
            err_check = WriteFile(file,
                      &path_size,
                      sizeof(path_size),
                      (DWORD*)&bytes_written,
                      NULL);
            
            if(!err_check)
                break;

            err_check = WriteFile(file,
                      &path[0],
                      path_size,
                      (DWORD*)&bytes_written,
                      NULL);

            if(!err_check)
                break;
        }

        CloseHandle(file);

        //if writing failed - delete file
        //as it is corrupted
        if(!err_check)
            DeleteFileA(FILE_DATA_SAVE_FULLPATH);
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
    //if there is no existing file
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

        if(!error_flag)
        {
            break;
        }
        else if(bytes_read < (int)sizeof(path_size))
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

        if(!error_flag)
        {
            break;
        }
        else if(shared_file.SetFile(path))
            shared_files_.push_back(shared_file);
    }

    CloseHandle(file);
}
