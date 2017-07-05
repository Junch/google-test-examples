#include <thread>
#include <memory>
#include <chrono>
#include <gmock/gmock.h>
using namespace std;

class A {
public:
    A():name("Hello World") {
    }

    void run() {
        std::this_thread::sleep_for(1s);
        printf("name=%s\n", name.c_str());
    }

    ~A() {
        printf("A:~A\n");
    }

    std::string name;
};

TEST(thread, risk)
{
    shared_ptr<A> pA = make_shared<A>();

    thread t([&pA]{
        pA->run();
    });

    pA.reset();
    t.join();
}

TEST(thread, ok)
{
    shared_ptr<A> pA = make_shared<A>();

    thread t([&pA] {
        shared_ptr<A> p = pA;
        p->run();
    });

    pA.reset();
    t.join();
}
