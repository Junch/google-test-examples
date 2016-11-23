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
    std::vector<int> v1{1, 2, 3, 4};
    IntVtrPtr vptr1 = make_shared<vector<IntPtr> >(vector<IntPtr>(v1.size()));
    std::transform(v1.begin(), v1.end(), vptr1->begin(), [](int i){
        return make_shared<int>(i);
    });

    std::vector<int> v2 = {2, 4, 5};
    IntVtrPtr vptr2 = make_shared<vector<IntPtr> >(vector<IntPtr>(v2.size()));
    std::transform(v2.begin(), v2.end(), vptr2->begin(), [](int i){
        return make_shared<int>(i);
    });

    IntVtrPtr added = make_shared<vector<IntPtr> >();
    IntVtrPtr removed = make_shared<vector<IntPtr> >();
    diff_ptr(vptr1, vptr2, added, removed);

    EXPECT_EQ(1, added->size());
    EXPECT_EQ(2, removed->size());
}

///////////////////////////////////////////////////////////////
//  ContactInfo
///////////////////////////////////////////////////////////////

struct ContactInfo
{
    std::string uri;
    std::string nickName;
    std::string groupName;
};

bool operator< (const ContactInfo& lhs, const ContactInfo& rhs) {
    if (lhs.uri < rhs.uri) return true;
    if (lhs.nickName < rhs.nickName) return true;
    if (lhs.groupName < rhs.groupName) return true;

    return false;
};

TEST(contactInfo_operator_smaller, add_and_remove_elements) {
    {
        ContactInfo info1 {"user_A@cisco.com", "nick_a", "group1"};
        ContactInfo info2 {"user_A@cisco.com", "nick_b", "group1"};
        EXPECT_TRUE(info1 < info2);
    }

    {
        ContactInfo info1 {"user_A@cisco.com", "nick_a", "group1"};
        ContactInfo info2 {"user_A@cisco.com", "nick_a", "group2"};
        EXPECT_TRUE(info1 < info2);
    }

    {
        ContactInfo info1 {"user_A@cisco.com", "nick_a", "group1"};
        ContactInfo info2 {"user_A@cisco.com", "nick_a", "group1"};
        EXPECT_TRUE(!(info1 < info2));
    }
}

TEST(contactInfo_operator_smaller, sort_elements) {
    typedef shared_ptr<ContactInfo> ContactInfoPtr;
    typedef vector<ContactInfoPtr> ContactInfoVtr;
    vector<ContactInfo> v = {
        {"user_B@cisco.com", "nick_b", "group2"},
        {"user_B@cisco.com", "nick_b", "group1"},
        {"user_A@cisco.com", "nick_a", "group1"}
    };

    ContactInfoVtr vv(v.size());

    std::transform(v.cbegin(), v.cend(), vv.begin(), [](const ContactInfo& info){
        return make_shared<ContactInfo>(info);
    });

    std::sort(vv.begin(), vv.end(), [](const ContactInfoPtr& ptr1, const ContactInfoPtr& ptr2){
        return *ptr1 < *ptr2;
    });

    std::for_each(vv.begin(), vv.end(), [](const ContactInfoPtr& infoPtr){
        std::cout << infoPtr->uri <<" "<< infoPtr->nickName << " " << infoPtr->groupName << "\n";
    });
}