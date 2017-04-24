#include "gtest/gtest.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

TEST(clock, test){
    cout << "system_clock" << endl;
    cout << system_clock::period::num << endl;
    cout << system_clock::period::den << endl;
    cout << "steady = " << boolalpha << system_clock::is_steady << endl << endl;

    cout << "high_resolution_clock" << endl;
    cout << high_resolution_clock::period::num << endl;
    cout << high_resolution_clock::period::den << endl;
    cout << "steady = " << boolalpha << high_resolution_clock::is_steady << endl << endl;

    cout << "steady_clock" << endl;
    cout << steady_clock::period::num << endl;
    cout << steady_clock::period::den << endl;
    cout << "steady = " << boolalpha << steady_clock::is_steady << endl << endl;
}
