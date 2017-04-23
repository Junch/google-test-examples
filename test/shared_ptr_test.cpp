#include "gtest/gtest.h"
#include <string>
#include <memory>

using namespace std;

void reset_ptr(shared_ptr<string> ptr)
{
    printf("use_count=%d\n", ptr.use_count());
    ptr.reset();
}
  
void reset_ptr_ref(shared_ptr<string>& ptr) // Wrong way to use shared_ptr
{
    printf("use_count=%d\n", ptr.use_count());
    ptr.reset();
}

TEST(shared_ptr, use_count)
{
    auto p1 = make_shared<string>("test shared_ptr");
    auto p2 = p1;
    reset_ptr(p2);
    printf("p1.use_count=%d p2.use_count2=%d\n", p1.use_count(), p2.use_count());
}

TEST(shared_ptr, ref_use_count)
{
    auto p1 = make_shared<string>("test shared_ptr");
    auto p2 = p1;
    reset_ptr_ref(p2);
    printf("p1.use_count=%d p2.use_count=%d\n", p1.use_count(), p2.use_count());
}
