#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/RecvSocket.h"
#include "../Chat/BroadcastSocket.h"

#include "TestDefines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(RecvSocket)
    {
    public:
        TEST_METHOD(Initialize_Close)
        {
            ::RecvSocket recv_sock;
            Assert::IsTrue(recv_sock.Initialize(TEST_PORT));
            Assert::IsTrue(recv_sock.Close());
        }
        TEST_METHOD(Recv)
        {
            ::BroadcastSocket br_sock(TEST_PORT);
            ::RecvSocket recv_sock;

            Assert::IsTrue(br_sock.Initialize());
            Assert::IsTrue(recv_sock.Initialize(TEST_PORT));

            char buffer[TEST_BROADCAST_MSG_SIZE] = TEST_BROADCAST_MSG;
            //return value (send bites) should be equal to msg size
            Assert::AreEqual(TEST_BROADCAST_MSG_SIZE,
                             br_sock.Send(buffer, TEST_BROADCAST_MSG_SIZE));

            RecvStruct recv_str;
            Assert::AreEqual(TEST_BROADCAST_MSG_SIZE,
                             recv_sock.Recv(&recv_str));

            Assert::IsTrue(recv_sock.Close());
            Assert::IsTrue(br_sock.Close());

            recv_str.Clear();
        }
    };
}