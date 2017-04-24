#include <gmock/gmock.h>
#include "stopwatch.h"
#include <chrono>
#include <thread>

TEST(stopwatch, test)
{
	using namespace std::chrono_literals;

	Stopwatch watch;
	watch.Start();
	std::this_thread::sleep_for(2s);
	watch.Stop();
	double elapsed= watch.ElapsedMilliseconds();
	printf("elapsed %.3f ms\n", elapsed);
}
