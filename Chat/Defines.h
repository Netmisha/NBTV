#ifndef DEFINES_H
#define DEFINES_H

//default port
#define PORT (unsigned short)20001
//port for tcp connectiong
#define FILE_PORT (unsigned short)12000
//broadcast ip
#define BROADCAST_IP "255.255.255.255"

//unix one is 1000, surprisingly
#define WINDOWS_KILOBYTE 1024

//max buffer size without
//possible data tearing
#define RECV_BUFFER_SIZE (WINDOWS_KILOBYTE * 4)
#define BUFFER_SIZE 512
#define CHUNK_SIZE 512

//size of ip in chars
#define IP_SIZE 16

//WSAStartup version define
#define WSA_MIN_VERSION 2
#define WSA_MAX_VERSION 2 

//0 for disabling broadcast loopback recv
//1 for enabling
#define BROADCAST_LOOPBACK 0

//ASCI buttons
#define BACKSPACE_BUTTON 8 

#define FILE_DATA_SAVE_DIR  "Data"
#define FILE_DATA_SAVE_FULLPATH "Data/shared_files.shif"

#define DOWNLOAD_DIR "Download"

#endif // !DEFINES_H