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
            ::UserMsg *unp_user_msg = (::UserMsg*)unp_msg.msg_;

            Assert::IsTrue((msg.color_ == unp_user_msg->color_) &&
                           (msg.name_ == unp_user_msg->name_) &&
                           (msg.type_ == unp_user_msg->type_) &&
                           (msg.msg_ == unp_user_msg->msg_));

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
            ::LogMessage *unp_log_msg = (::LogMessage*)unp_msg.msg_;
            
            Assert::IsTrue((msg.color_ == unp_log_msg->color_) &&
                           (msg.name_ == unp_log_msg->name_) &&
                           (msg.prev_name_ == unp_log_msg->prev_name_) &&
                           (msg.type_ == unp_log_msg->type_));

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
            Assert::IsTrue(memcmp(&rand_index, unp_msg.msg_, sizeof(rand_index)) == 0);

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
            Assert::AreEqual(file.GetSizeKB(), (*(RecvFileInfo*)unp_msg.msg_).size_KB_);

            delete[] packed_message;
            unp_msg.Clear();
        }
    };
}