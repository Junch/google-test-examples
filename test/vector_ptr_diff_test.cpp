#include "gtest/gtest.h"
#include <vector>
#include <memory>
using namespace std;
typedef std::shared_ptr<int> IntPtr;
typedef std::shared_ptr<vector<IntPtr> > IntVtrPtr;

void diff_ptr(const IntVtrPtr v1, const IntVtrPtr v2, IntVtrPtr added, IntVtrPtr removed){
    auto iter1 = v1->cbegin();
    auto iter2 = v2->cbegin();

    while (true) {
        if (iter1 == v1->cend()) {
            std::copy(iter2, v2->cend(), std::back_inserter(*added));
            return;
        }

        if (iter2 == v2->cend()){
            std::copy(iter1, v1->cend(), std::back_inserter(*removed));
            return;
        }

        if (**iter1 == **iter2){
            ++iter1;
            ++iter2;
        }else if (**iter1 < **iter2) {
            removed->push_back(*iter1);
            ++iter1;
        }else {
            added->push_back(*iter2);
            ++iter2;
        }
    }
}

TEST(vector_diff_ptr_test, add_and_remove_elements){
    IntVtrPtr vv1 = make_shared<vector<IntPtr> >();
    {
        std::vector<int> v1 = {1, 2, 3, 4};
        for(const auto i: v1){
            vv1->push_back(make_shared<int>(i));
        }
    }

    IntVtrPtr vv2 = make_shared<vector<IntPtr> >();
    {
        std::vector<int> v2 = {2, 4, 5};
        for(const auto i: v2){
            vv2->push_back(make_shared<int>(i));
        }
    }

    IntVtrPtr added = make_shared<vector<IntPtr> >();
    IntVtrPtr removed = make_shared<vector<IntPtr> >();
    diff_ptr(vv1, vv2, added, removed);

    EXPECT_EQ(1, added->size());
    EXPECT_EQ(2, removed->size());
}
