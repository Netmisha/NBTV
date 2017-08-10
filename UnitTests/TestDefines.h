#ifndef TEST_DEFINES_H
#define TEST_DEFINES_H

#define TEST_BROADCAST_PORT ((unsigned int)12345)
#define TEST_TCP_PORT ((unsigned int)12321)
#define TEST_BROADCAST_MSG_SIZE (7)
#define TEST_BROADCAST_MSG ("OMGWTF")
#define TEST_LOCALHOST_IP ("127.0.0.1")

#define TEST_CHAT_MSG(type) {type, 1, "Dio", "ZA WARUDO"}
#define TEST_CHAT_MSG_SIZE (17)

#define TEST_LOG_MSG {LOG_UPDATE, "Dio", "Brando", (char)1}
#define TEST_LOG_MSG_SIZE (14)

#define TEST_FILE_SEND_PATH ("test.txt")

#define TEST_FILE_ALWAYS_EXISTS_FULLPATH ("../../UnitTests/TestDefines.h")
#define TEST_FILE_ALWAYS_EXISTS_NAME ("TestDefines.h")

#endif // !TEST_DEFINES_H