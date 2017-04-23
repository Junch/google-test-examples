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
