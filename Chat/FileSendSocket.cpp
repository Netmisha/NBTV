#include "FileSendSocket.h"
#include <Mswsock.h>
#pragma comment(lib, "mswsock.lib")

FileSendSocket::FileSendSocket(){}


FileSendSocket::~FileSendSocket(){}

bool FileSendSocket::Initialize()
{
	if (!AbstractSocket::Initialize(TCP))
	{
		return false;
	}
       
	return true;
}

bool FileSendSocket::SendFile(std::string pass, std::string ip)
{
    if (socket_ == INVALID_SOCKET)
        exit(1);

    sockaddr_in sock_addr;

    //set server port and IP
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(FILE_PORT); //Host TO Network Short
    sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    
    //connect 
    if (connect(socket_, (sockaddr*)(&sock_addr), sizeof(sock_addr)) != 0)
    {
        std::cerr << "Unable to connect!\n";
    }

    HANDLE file_to_send = CreateFile (TEXT(pass.c_str()) ,
        GENERIC_READ, //I will read from this file
        0, NULL,
        CREATE_ALWAYS, //Creates a new file, always.
        FILE_ATTRIBUTE_NORMAL, NULL);
    bool result = (bool)TransmitFile(socket_, file_to_send,
                                     0, //we send hole file
                                     0, //default  nNumberOfBytesPerSend
                                     NULL, //Overlapped
                                     NULL,
                                     (BOOL)TF_DISCONNECT); //  disconnect after all 
                                    //the file data has been send
    return result;
}
