#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/Parcer.h"

#include "TestDefines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(Parcer)
    {
    public:
        TEST_METHOD(PrepareMessage)
        {
            int rand_value = rand() % 1000;
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(PREPARE_MESSAGE, &rand_value, packed_message);
            Assert::AreEqual((int)PREPARE_MESSAGE_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)PREPARE_MESSAGE);
            Assert::AreEqual(rand_value, *(int*)unp_msg.msg_);
            
            delete[] packed_message;
            unp_msg.Clear();
        }
        TEST_METHOD(UserMsg)
        {
            ::UserMsg msg = TEST_CHAT_MSG;
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(CHAT_MESSAGE, &msg, packed_message);
            Assert::AreEqual(TEST_CHAT_MSG_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)CHAT_MESSAGE);
            Assert::AreEqual(msg.msg_, (*(::UserMsg*)unp_msg.msg_).msg_);

            delete[] packed_message;
            unp_msg.Clear();
        }
        TEST_METHOD(LogMessage)
        {
            ::LogMessage msg = TEST_LOG_MSG;
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(LOG_MESSAGE, &msg, packed_message);
            Assert::AreEqual(TEST_LOG_MSG_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)LOG_MESSAGE);
            Assert::AreEqual(msg.name_, (*(::LogMessage*)unp_msg.msg_).name_);

            delete[] packed_message;
            unp_msg.Clear();
        }
        TEST_METHOD(GetFileMessage)
        {
            int rand_index = rand() % 100;
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(GET_FILE_MESSAGE, &rand_index, packed_message);
            Assert::AreEqual((int)GET_FILE_MESSAGE_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)GET_FILE_MESSAGE);
            Assert::AreEqual(rand_index, *(int*)unp_msg.msg_);

            delete[] packed_message;
            unp_msg.Clear();
        }
        TEST_METHOD(FileListRequest)
        {
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(FILE_LIST_REQUEST, NULL, packed_message);
            Assert::AreEqual((int)FILE_LIST_REQUEST_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)FILE_LIST_REQUEST);

            delete[] packed_message;
            unp_msg.Clear();
        }
        TEST_METHOD(FileListMessage)
        {
            File file("ParcerTests.cpp");
            void* packed_message;
            int msg_size = ::Parcer::PackMessage(FILE_LIST_MESSAGE, &file, packed_message);
            Assert::AreEqual((int)FILE_LIST_MESSAGE_SIZE, msg_size);
            UnpackedMessage unp_msg = ::Parcer::UnpackMessage(packed_message);
            Assert::AreEqual((int)unp_msg.type_, (int)FILE_LIST_MESSAGE);
            Assert::AreEqual(file.GetName(), (*(RecvFileInfo*)unp_msg.msg_).name_);

            delete[] packed_message;
            unp_msg.Clear();
        }
    };
}