#include "gtest/gtest.h"
#include <vector>
using namespace std;

void diff(const vector<int>& v1, const vector<int>& v2, vector<int>& added, vector<int>& removed){
    size_t i = 0, j = 0;

    while (i < v1.size() && j < v2.size()) {
        if (v1[i] == v2[j]){
            ++i;
            ++j;
        }else if (v1[i] < v2[j]) {
            removed.push_back(v1[i]);
            ++i;
        }else {
            added.push_back(v2[j]);
            ++j;
        }
    }

    while (i < v1.size()) {
        removed.push_back(v1[i]);
        ++i;
    }

    while (j < v2.size()) {
        added.push_back(v2[j]);
        ++j;
    }
}

TEST(vector_diff_test, add_and_remove_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = {2, 4, 5};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(1, added.size());
    EXPECT_EQ(2, removed.size());
}

TEST(vector_diff_test, same_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = {1, 2, 3, 4};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(0, added.size());
    EXPECT_EQ(0, removed.size());
}

TEST(vector_diff_test, remove_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2;
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(0, added.size());
    EXPECT_EQ(4, removed.size());
}

TEST(vector_diff_test, add_elements){
    std::vector<int> v1;
    std::vector<int> v2 = {1, 2, 3, 4};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(4, added.size());
    EXPECT_EQ(0, removed.size());
}
