#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/TCPSocket.h"
#include "../Chat/Thread.h"

#include "TestDefines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

unsigned AcceptFunc(void *sock)
{
    TCPSocket s;
    (*(TCPSocket*)sock).Accept(s);
    s.Close();
    return 0;
}

namespace UnitTests
{
    TEST_CLASS(RecvSocket)
    {
    public:
        TEST_METHOD(Initialize_Close)
        {
            ::TCPSocket tcp_sock;
            Assert::IsTrue(tcp_sock.Initialize());
            Assert::IsTrue(tcp_sock.Close());
        }
        TEST_METHOD(Accept)
        {
            ::TCPSocket listen_sock;
            Assert::IsTrue(listen_sock.Initialize());
            Assert::IsTrue(listen_sock.SetListen(TEST_BROADCAST_PORT, 1));

            ::TCPSocket connecting_sock;
            Assert::IsTrue(connecting_sock.Initialize());
            Thread th(AcceptFunc, &listen_sock);
            connecting_sock.Connect(TEST_LOCALHOST_IP, TEST_BROADCAST_PORT);
            th.Join();

            Assert::IsTrue(connecting_sock.Close());
            Assert::IsTrue(listen_sock.Close());
        }
    };
}