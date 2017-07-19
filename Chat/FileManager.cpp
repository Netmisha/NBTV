#include "FileManager.h"

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
    shared_files_.erase(shared_files_.begin() + file_index);
}

std::string FileManager::GetFilePath(int file_index)const
{
    return shared_files_[file_index].GetPath();
}

std::string FileManager::GetFileName(int file_index)const
{
    return shared_files_[file_index].GetName();
}

std::vector<std::string> FileManager::GetFilePaths()const
{
    std::vector<std::string> return_vector;
    for(File it : shared_files_)
    {
        return_vector.push_back(it.GetPath());
    }
    return return_vector;
}

std::vector<std::string> FileManager::GetFileNames()const
{
    std::vector<std::string> return_vector;
    for(File it : shared_files_)
    {
        return_vector.push_back(it.GetName());
    }
    return return_vector;
}

void FileManager::Save()
{
    if((CreateDirectory(FILE_DATA_SAVE_DIR, NULL)) ||
       (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        std::ofstream file_stream;
        file_stream.open(FILE_DATA_SAVE_FULLPATH,
                         std::ofstream::out);

        for(std::string path : GetFilePaths())
        {
            file_stream << path.length() << path;
        }
    }
}

void FileManager::Load()
{
    std::ifstream file_stream(FILE_DATA_SAVE_FULLPATH,
                              std::ifstream::in);
    if(!file_stream.is_open())
    {
        return;
    }

    int path_size;
    std::string path;
    File file;
    while(!file_stream.eof())
    {
        file_stream >> path_size;
        path.resize(path_size);
        file_stream.read(&path[0], path_size);
        if(file.SetFile(path))
            shared_files_.push_back(file);
    }
}