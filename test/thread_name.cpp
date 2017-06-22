#include <gmock/gmock.h>
using namespace std;

string getThreadIdLast16bits(uint64_t id){
	ostringstream oss;
	oss << hex << id;
	const string& content = oss.str();
	int length = content.length();
	return length < 4? content: content.substr(length-4, length);
}

string getPrintName(std::string threadName, uint64_t id)
{
	if(threadName.length() == 0){
		return getThreadIdLast16bits(id);
	}

	if (threadName.length() > 10){
		threadName = threadName.substr(0, 10);
	}

	ostringstream oss;
	oss << threadName << '-' << getThreadIdLast16bits(id);

	return oss.str();
}

TEST(Thread, general) {
	string threadName = "1234567890AB";
	uint64_t id = 0x1234567F;
	string newName = getPrintName(threadName, id);
	EXPECT_STREQ("1234567890-567f", newName.c_str());
}

TEST(Thread, id_is_short) {
	string threadName = "1234567890AB";
	uint64_t id = 0xA78;
	string newName = getPrintName(threadName, id);
	EXPECT_STREQ("1234567890-a78", newName.c_str());
}

TEST(Thread, name_is_empty) {
	string threadName = "";
	uint64_t id = 0x6789F;
	string newName = getPrintName(threadName, id);
	EXPECT_STREQ("789f", newName.c_str());
}
