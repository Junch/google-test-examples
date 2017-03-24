#include <gmock/gmock.h>
#include "logger.h"
using namespace std;

TEST(Logger, test) {
	CLogger::GetLogger()->Log("Hello world");
	CLogger::GetLogger()->Log(string("Nice to meet you"));
	LOGGER->Log("%s is %d years old", "Tom", 40);
}
