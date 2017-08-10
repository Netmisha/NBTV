#include "stdafx.h"
#include "CppUnitTest.h"

#define DLL_IMPORT
#include "../Chat/File.h"

#include "TestDefines.h"

#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(File)
    {
    public:
        TEST_METHOD(SetFile)
        {
            ::File test_file;
            Assert::IsTrue(test_file.SetFile(TEST_FILE_ALWAYS_EXISTS_FULLPATH));
            Assert::AreEqual(test_file.GetName(), std::string(TEST_FILE_ALWAYS_EXISTS_NAME));
        }
    };
}