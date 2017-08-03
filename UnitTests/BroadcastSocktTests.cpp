#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/BroadcastSocket.h"

#include "TestDefines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(BroadcastSocket)
	{
	public:
        TEST_METHOD(Initialize_Close)
        {
            ::BroadcastSocket br_sock(TEST_PORT);
            Assert::IsTrue(br_sock.Initialize());
            Assert::IsTrue(br_sock.Close());
        }
        TEST_METHOD(Send)
        {
            ::BroadcastSocket br_sock(TEST_PORT);
            char buffer[TEST_BROADCAST_MSG_SIZE] = TEST_BROADCAST_MSG;
            br_sock.Initialize();
            //return value (send bites) should be equal to msg size
            Assert::AreEqual(TEST_BROADCAST_MSG_SIZE,
                             br_sock.Send(buffer, TEST_BROADCAST_MSG_SIZE));
        }
        TEST_METHOD(SendTo)
        {
            ::BroadcastSocket br_sock(TEST_PORT);
            char buffer[TEST_BROADCAST_MSG_SIZE] = TEST_BROADCAST_MSG;
            br_sock.Initialize();
            //return value (send bites) should be equal to msg size
            Assert::AreEqual(TEST_BROADCAST_MSG_SIZE,
                             br_sock.SendTo(buffer,
                             TEST_BROADCAST_MSG_SIZE,
                             TEST_LOCALHOST_IP));
        }
	};
}