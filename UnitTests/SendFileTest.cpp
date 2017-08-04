#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/FileSendSocket.h"
#include "../Chat/FileGetSocket.h"
#include "../Chat/Thread.h"

#include "TestDefines.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(GetFile)
    {
    public:
        TEST_METHOD(SendFile)
        {
            HANDLE file = CreateFileA(TEST_FILE_SEND_PATH,   //path
                                      GENERIC_WRITE,             //to write
                                      0,                         //non-share
                                      NULL,                      //security
                                      CREATE_ALWAYS,             //always create
                                      FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                                      NULL);                     //why no default arguments
            Assert::AreNotEqual(file, INVALID_HANDLE_VALUE);
            char buffer[TEST_BROADCAST_MSG_SIZE] = TEST_BROADCAST_MSG;
            int bytes_written;
            Assert::IsTrue(WriteFile(file,
                &buffer,
                TEST_BROADCAST_MSG_SIZE,
                (DWORD*)&bytes_written,
                NULL));

            CloseHandle(file);

            FileGetSocket file_sock;
            Assert::IsTrue(file_sock.Initialize(TEST_BROADCAST_PORT));
            Thread th(FileGetSocket::GetFileStartup, &file_sock);

            FileSendSocket file_send_sock;
            Assert::IsTrue(file_send_sock.SendFile(TEST_FILE_SEND_PATH,
                                                   TEST_LOCALHOST_IP,
                                                   TEST_FILE_SEND_PATH,
                                                   TEST_BROADCAST_PORT));

            th.Join();
            DeleteFileA(TEST_FILE_SEND_PATH);
            std::string download_loc(std::string(DOWNLOAD_DIR) + "/" + TEST_FILE_SEND_PATH);
            Assert::IsTrue((bool)PathFileExistsA(download_loc.c_str()));
            char recv_buffer[TEST_BROADCAST_MSG_SIZE] = { 0 };
            
            file = CreateFileA(download_loc.c_str(),      //path
                               GENERIC_READ,              //to write
                               0,                         //non-share
                               NULL,                      //security
                               OPEN_EXISTING,             //always create
                               FILE_ATTRIBUTE_NORMAL,     //nothing-specific-file
                               NULL);                     //why no default arguments
            Assert::AreNotEqual(file, INVALID_HANDLE_VALUE);
            Assert::IsTrue(ReadFile(file,
                recv_buffer,
                TEST_BROADCAST_MSG_SIZE,
                (DWORD*)&bytes_written,
                NULL));
            Assert::IsTrue(strcmp(buffer, recv_buffer) == 0);
        }
    };
}