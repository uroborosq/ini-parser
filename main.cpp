#include <iostream>
#include "ini.h"
using namespace std;

int main() {
    IniFile minecraft_pro;
    minecraft_pro.open("test.ini");
    minecraft_pro.get("section1")->add("122112325", "1");
    minecraft_pro.add("samurai")->add("hru", "hru");

    minecraft_pro.remove("new section");

    minecraft_pro.get("qdqwad")->add("wso", "2");

    vector<IniSection> o = minecraft_pro.get_full();

    return 0;
}
