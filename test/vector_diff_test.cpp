#include "gmock/gmock.h"
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

    ASSERT_THAT(added, ::testing::ElementsAre(5));
    ASSERT_THAT(removed, ::testing::ElementsAre(1,3));
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

////////////////////////////////////////////////////////////////
// Use iterator instead of index
////////////////////////////////////////////////////////////////

void diff_iter(const vector<int>& v1, const vector<int>& v2, vector<int>& added, vector<int>& removed){
    auto iter1 = v1.cbegin();
    auto iter2 = v2.cbegin();

    while (true) {
        if (iter1 == v1.cend()) {
            std::copy(iter2, v2.cend(), std::back_inserter(added));
            return;
        }

        if (iter2 == v2.cend()){
            std::copy(iter1, v1.cend(), std::back_inserter(removed));
            return;
        }

        if (*iter1 == *iter2){
            ++iter1;
            ++iter2;
        }else if (*iter1 < *iter2) {
            removed.push_back(*iter1);
            ++iter1;
        }else {
            added.push_back(*iter2);
            ++iter2;
        }
    }
}

TEST(vector_diff_iter_test, add_and_remove_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = {2, 4, 5};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);

    ASSERT_THAT(added, ::testing::ElementsAre(5));
    ASSERT_THAT(removed, ::testing::ElementsAre(1,3));
}

TEST(vector_diff_iter_test, same_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = {1, 2, 3, 4};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(0, added.size());
    EXPECT_EQ(0, removed.size());
}

TEST(vector_diff_iter_test, remove_elements){
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2;
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(0, added.size());
    EXPECT_EQ(4, removed.size());
}

TEST(vector_diff_iter_test, add_elements){
    std::vector<int> v1;
    std::vector<int> v2 = {1, 2, 3, 4};
    std::vector<int> added, removed;
    diff(v1, v2, added, removed);
    EXPECT_EQ(4, added.size());
    EXPECT_EQ(0, removed.size());
}
