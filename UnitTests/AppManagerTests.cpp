#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/AppManager.h"

#include "TestDefines.h"

#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(AppManager)
    {
    public:
        TEST_METHOD(AddFile_RemoveFile)
        {
            ::AppManager app_manager(TEST_BROADCAST_PORT, TEST_TCP_PORT);
            app_manager.ActivateCommand(std::string("/addf NBTV.dll"));

            std::vector<File> *file_names = (std::vector<File>*)
                app_manager.ActivateCommand(std::string("/fl "));

            Assert::AreEqual((*file_names)[0].GetName(), std::string("NBTV.dll"));

            app_manager.ActivateCommand(std::string("/removef 1"));
        }
        TEST_METHOD(ChatMessagePublic)
        {
            ::AppManager app_manager(TEST_BROADCAST_PORT, TEST_TCP_PORT);
            
            BroadcastSocket bc_socket;
            bc_socket.Initialize(TEST_BROADCAST_PORT);

            UserMsg msg = TEST_CHAT_MSG(PUBLIC);
            void *packet = NULL;
            int size = Parcer::PackMessage(CHAT_MESSAGE, &msg, packet);
            bc_socket.SendTo(packet, size, TEST_LOCALHOST_IP);
            delete[] packet;

            while(true)
            {
                UnpackedMessage unp_msg = app_manager.RecieveMessage();
                if(unp_msg.type_ == CHAT_MESSAGE)
                    break;
            }

            const std::vector<UserMsg>& msgs = *app_manager.GetPrivateChatMsgs(PUBLIC_MSGS);
            Assert::IsTrue((msgs[0].color_ == msg.color_) &&
                           (msgs[0].msg_ == msg.msg_) &&
                           (msgs[0].name_ == msg.name_) &&
                           (msgs[0].type_ == msg.type_));
        }
        TEST_METHOD(ChatMessagePrivate)
        {
            ::AppManager app_manager(TEST_BROADCAST_PORT, TEST_TCP_PORT);

            BroadcastSocket bc_socket;
            bc_socket.Initialize(TEST_BROADCAST_PORT);

            UserMsg msg = TEST_CHAT_MSG(PRIVATE);
            void *packet = NULL;
            int size = Parcer::PackMessage(CHAT_MESSAGE, &msg, packet);
            bc_socket.SendTo(packet, size, TEST_LOCALHOST_IP);
            delete[] packet;

            while(true)
            {
                UnpackedMessage unp_msg = app_manager.RecieveMessage();
                if(unp_msg.type_ == CHAT_MESSAGE)
                    break;
            }

            const std::vector<UserMsg> &msgs = *app_manager.GetPrivateChatMsgs(msg.name_);
            Assert::IsTrue((msgs[0].color_ == msg.color_) &&
                           (msgs[0].msg_ == msg.msg_) &&
                           (msgs[0].name_ == msg.name_) &&
                           (msgs[0].type_ == msg.type_));
        }
    };
}