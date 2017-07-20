#include "FileGetSocket.h"


FileGetSocket::FileGetSocket(){}


FileGetSocket::~FileGetSocket(){}

bool FileGetSocket::Initialize()
{
    if (!AbstractSocket::Initialize(TCP))
    {
        return false;
    }

    return true;
}

void FileGetSocket::GetFile(const std::string& filename)
{
    sockaddr_in sock_addr = { AF_INET, htons(FILE_PORT), INADDR_ANY };



    if (bind(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
    {
        std::cerr << "unable to bind socket!\n";
        shutdown(socket_, 2);
    }

    if (listen(socket_, SOMAXCONN) != 0)
    {
        std::cerr << "unable to set listening socket mode!\n";
        shutdown(socket_, 2);
    }

    SOCKET file_getter = accept(socket_, 0, 0);

    HANDLE hFile = CreateFile( (LPCTSTR)filename.c_str() , 
                            GENERIC_WRITE, //I will write in this file
                            0, NULL, 
                            CREATE_ALWAYS, //Creates a new file, always.
                            FILE_ATTRIBUTE_NORMAL, NULL);
    
    LPVOID buffer;// aka void*
    LPDWORD transferred_bytes=NULL; //pointer to a variable that receives the number 
                    //of bytes that were actually transferred by a read or write operation
    LPOVERLAPPED overlap_struct=NULL; //contains info used in asynchronous  (I/O).
    while (true)
    {
        ReadFile((HANDLE)socket_, &buffer, MAX_PATH, transferred_bytes, overlap_struct);
        GetOverlappedResult((HANDLE)socket_, overlap_struct, transferred_bytes, TRUE);
        if (transferred_bytes == 0) break;
        WriteFile(hFile, buffer, *transferred_bytes, transferred_bytes, NULL);
    }
    CloseHandle(hFile);
}
