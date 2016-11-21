#include "gtest/gtest.h"
#include <vector>
using namespace std;

void diff(const vector<int>& v1, const vector<int>& v2, vector<int>& added, vector<int>& removed){

}

TEST(vector_diff_test, add_and_remove_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = {2, 4, 5};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(1, added.size());
    EXPECT_EQ(2, removed.size());
}
