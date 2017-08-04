#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/AppManager.h"

#include "TestDefines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(AppManager)
    {
    public:
        TEST_METHOD(AddFile)
        {
            ::AppManager app_manager(TEST_BROADCAST_PORT, TEST_TCP_PORT);
            app_manager.ActivateCommand(std::string("/addf NBTV.dll"));

            std::vector<File> *file_names = (std::vector<File>*)
                app_manager.ActivateCommand(std::string("/fl "));

            Assert::AreEqual((*file_names)[0].GetName(), std::string("NBTV.dll"));
        }
    };
}