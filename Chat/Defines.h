#ifndef DEFINES_H
#define DEFINES_H

#include <string>

//default port
#define PORT ((unsigned short)20001)
//port for tcp connectiong
#define FILE_PORT ((unsigned short)12000)
//broadcast ip
#define BROADCAST_IP ("255.255.255.255")

//unix one is 1000, surprisingly
#define WINDOWS_KILOBYTE (1024)

//max buffer size without
//possible data tearing
#define RECV_BUFFER_SIZE (WINDOWS_KILOBYTE)
#define BUFFER_SIZE (512)
#define CHUNK_SIZE (512)

//size of ip in chars
#define IP_SIZE (16)

//WSAStartup version define
#define WSA_MIN_VERSION (2)
#define WSA_MAX_VERSION (2) 

//0 for disabling broadcast loopback recv
//1 for enabling
#define BROADCAST_LOOPBACK (0)

//dir where shared files data is stored
#define DATA_SAVE_DIR ("Data")
//full path to file where shared files data is stored
#define FILE_DATA_SAVE_FULLPATH ("Data/shared_files.shif")
//full path to file where user info (name/color) is stored
#define USER_DATA_SAVE_FULLPATH ("Data/user_info.urio")

//default dir where downloaded shared files are located
#define DOWNLOAD_DIR ("Download")

#define DLL_EXP __declspec(dllexport)

//key value for public messages vector in Chat object
const std::string PUBLIC_MSGS("PUBLIC_MSGS");

//maximum msgs stored from a single chat
#define MAX_MSGS_STORED (50)

#define DEFAULT_COLOR (char(1))

#endif // !DEFINES_H