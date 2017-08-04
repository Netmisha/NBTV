#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "File.h"
#include "Defines.h"

#include <vector>
#include <string>

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
    const std::string GetFilePath(int file_index)const;
    //get specific file name
    const std::string GetFileName(int file_index)const;
    //get size in kilobytes
    const double GetFileSizeKB(int file_index)const;
    //get size in megabytes
    const double GetFileSizeMB(int file_index)const;
    //get File struct
    bool GetFile(int file_index, File &out_result)const;
    //get all file paths
    void GetFilePaths(std::vector<std::string> &out_result)const;
    //get all file names
    void GetFileNames(std::vector<std::string> &out_result)const;
    //get vector of all files
    const std::vector<File>& GetFiles()const;

private:
    //vector of shared files
    std::vector<File> shared_files_;

    //saves shared file info
    void Save()const;
    //loads shared file info
    void Load();
};

#endif // !FILE_MANAGER_H;