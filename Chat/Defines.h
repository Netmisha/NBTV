#ifndef DEFINES_H
#define DEFINES_H

//default port
#define PORT (unsigned short)20001
//broadcast ip
#define BROADCAST_IP "255.255.255.255"
//max buffer size without
//possible data tearing
#define BUFFER_SIZE 512
//size of ip in chars
#define IP_SIZE 16

//WSAStartup version define
#define WSA_MIN_VERSION 2
#define WSA_MAX_VERSION 2
#define WSA_VERSION MAKEWORD(WSA_MIN_VERSION, WSA_MAX_VERSION) 

//0 for disabling broadcast loopback recv
//1 for enabling
#define BROADCAST_LOOPBACK 0

#endif // !DEFINES_H