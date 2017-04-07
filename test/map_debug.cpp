#include <gmock/gmock.h>
#include <map>
#include <string>
using namespace std;

typedef map<int, string> MyMap;

MyMap getMap(){
    MyMap mymap;
    mymap[1] = "Map1";
    mymap[2] = "Map2";
    mymap[3] = "Map3";
    mymap[4] = "Map4";
    mymap[5] = "Map5";

    return mymap;
}

TEST(MAP, test){
    // Use windbg to track mymap in memory
    MyMap mymap = getMap();
    for (auto& item: mymap) {
        printf("%d: %s\n", item.first, item.second.c_str());
    }
}
