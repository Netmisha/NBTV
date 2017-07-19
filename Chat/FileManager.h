#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "File.h"
#include "Defines.h"

#include <Windows.h>

#include <vector>
#include <string>
#include <fstream>

class FileManager
{
public:
    FileManager();
    ~FileManager();

    //add file by path
    bool AddFile(const std::string &path);
    //remove file by index
    void RemoveFile(int file_index);
    //get specific file path
    std::string GetFilePath(int file_index)const;
    //get specific file name
    std::string GetFileName(int file_index)const;
    //get all file paths
    std::vector<std::string> GetFilePaths()const;
    //get all file names
    std::vector<std::string> GetFileNames()const;

private:
    //vector of shared files
    std::vector<File> shared_files_;

    void Save();
    void Load();
};

#endif // !FILE_MANAGER_H