#include <gmock/gmock.h>
#include <map>
#include <vector>
#include <list>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

using MyMap = map<int, string>;
using MyVec = vector<string>;
using MyList = list<string>;

MyMap getMap(){
    MyMap mymap;
    mymap[1] = "Map1";
    mymap[2] = "Map2";
    mymap[3] = "Map3";
    mymap[4] = "Map4";
    mymap[5] = "Map5";

    return mymap;
}

MyVec getVector(){
    MyVec myvec;
    myvec.push_back("Vec1");
    myvec.push_back("Vec2");
    myvec.push_back("Vec3");
    myvec.push_back("Vec4");
    myvec.push_back("Vec5");

    return myvec;
}

MyList getList(){
    MyList mylist;
    mylist.push_back("List1");
    mylist.push_back("List2");
    mylist.push_back("List3");
    mylist.push_back("List4");
    mylist.push_back("List5");

    return mylist;
}


TEST(MAP, test){
#ifdef _WIN32
    if (IsDebuggerPresent()) {
        DebugBreak();
    }
#endif

    MyMap mymap = getMap();
    MyVec myvec = getVector();
    MyList mylist = getList();

    for (auto& item: mymap) {
        printf("map: %d -> %s\n", item.first, item.second.c_str());
    }

    for (auto& item: myvec) {
        printf("vector: %s\n", item.c_str());
    }
    printf("vector: size: %u, capacity: %u\n", myvec.size(), myvec.capacity());

    for (auto& item: mylist) {
        printf("list: %s\n", item.c_str());
    }
}

class A {
public:
    A(){
        printf("A::A()\n");
    }

    A(std::string name):m_name(std::move(name)){
        printf("A::A(std::string)\n");
    }

    A(const A& a){
        printf("A::A(const A&)\n");
        m_name = a.m_name;
    }

    A(A&& a){
        printf("A::A(A&&)\n");
        m_name = std::move(a.m_name);
    }

    A &A::operator =(const A& a){
        printf("A::operator =\n");
        if (this == &a){
            return *this;
        }

        m_name = a.m_name;
        return *this;
    }

    ~A(){
        printf("A::~A\n");
    }

    std::string m_name;
};

TEST(MAP, tranverse)
{
using Container = map<int, A>;
    Container container;
    container.emplace(1, "item1");
    container.emplace(2, "item2");

    printf("================\n");

    for(const auto iter: container){
        std::cout << iter.second.m_name << '\n';
    };

    printf("================\n");
}

TEST(MAP, tranverse_op)
{
using Container = map<int, A>;
    Container container;
    container.emplace(1, "item1");
    container.emplace(2, "item2");

    printf("================\n");

    for(const auto& iter: container){
        std::cout << iter.second.m_name << '\n';
    };

    printf("================\n");
}

TEST(MAP, insert1)
{
using Container = map<int, A>;
    Container container;
    container.insert(std::pair<int, A>(1, A("item1")));
    container.insert(std::pair<int, A>(2, A("item2")));

    printf("================\n");
}

TEST(MAP, insert2)
{
using Container = map<int, A>;
    Container container;
    container[1] = A("item1");
    container[2] = A("item2");

    printf("================\n");
}

TEST(MAP, insert3)
{
using Container = map<int, A>;
    A a1("item1");
    A a2("item2");

    printf("================\n");
    Container container;
    container[1] = a1;
    container[2] = a2;

    printf("================\n");
}

TEST(MAP, insert4)
{
using Container = map<int, A>;
    A a1("item1");
    A a2("item2");

    printf("================\n");
    Container container;
    container.emplace(1, a1);
    container.emplace(2, a2);

    std::cout << "a1: " << a1.m_name << '\n';
    printf("================\n");
}

TEST(MAP, insert5)
{
using Container = map<int, A>;
    A a1("item1");
    A a2("item2");

    printf("================\n");
    Container container;
    container.emplace(1, std::move(a1));
    container.emplace(2, std::move(a2));

    std::cout << "a1: " << a1.m_name << '\n';
    printf("================\n");
}

TEST(VECTOR, push1)
{
using Container = vector<A>;
    A a1("item1");
    A a2("item2");
    A a3("item3");

    printf("================\n");
    Container container(4);
    container[0] = a1;
    container[1] = a2;
    container[2] = a3;

    printf("================\n");
}

TEST(VECTOR, push2)
{
using Container = vector<A>;
    A a1("item1");
    A a2("item2");
    A a3("item3");

    printf("================\n");
    Container container;
    container.emplace_back(a1);
    container.emplace_back(a2);
    container.emplace_back(a3);

    printf("================\n");
}

TEST(VECTOR, push3)
{
using Container = vector<A>;
    A a1("item1");
    A a2("item2");
    A a3("item3");

    printf("================\n");
    Container container;
    container.push_back(a1);
    container.push_back(a2);
    container.push_back(a3);

    printf("================\n");
}

TEST(VECTOR, push4)
{
using Container = vector<A>;
    Container container;
    container.push_back(A("item1"));
    container.push_back(A("item2"));
    container.push_back(A("item3"));

    printf("================\n");
}

TEST(VECTOR, push5)
{
using Container = vector<A>;
    Container container;
    container.emplace_back("item1");
    container.emplace_back("item2");
    container.emplace_back("item3");

    printf("================\n");
}
