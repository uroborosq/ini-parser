#include <iostream>
#include "ini.h"
using namespace std;

int main() {
    IniFile minecraft_pro;
    minecraft_pro.open("prmFile.prm");
//    minecraft_pro.get("section1")->add("122112325", "1");
//    minecraft_pro.add("samurai")->add("hru", "hru");
//
//    minecraft_pro.remove("section1");
//
//   // minecraft_pro.get("qdqwad")->add("wso", "2");
//
//    vector<IniSection> o = minecraft_pro.get_full();
//minecraft_pro.add("section")->add("privet", "poka");
    //minecraft_pro.get("section")->remove("privet");
cout << minecraft_pro.get("emc")->get("aaa")->second;

    return 0;
}
