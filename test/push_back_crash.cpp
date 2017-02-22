#include "gtest/gtest.h"
#include <vector>
#include <memory>
using namespace std;
typedef std::shared_ptr<int> IntPtr;
typedef std::shared_ptr<vector<IntPtr> > IntVtrPtr;

TEST(push_back_crash, example) {
    IntVtrPtr vPtr1 = make_shared<vector<IntPtr>>();
    IntVtrPtr vPtr2 = make_shared<vector<IntPtr>>();

    ASSERT_DEATH(vPtr2->push_back(vPtr1->front()), "");
}
