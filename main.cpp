#include <iostream>
#include "ini.h"
#include <fstream>
using namespace std;

int main() {
    IniFile minecraft_pro;
    minecraft_pro.open("test.ini");
    minecraft_pro.add("new section");
    minecraft_pro.get("new section")->second.add("horosho", "ploho");
    minecraft_pro.get("section1")->second.remove("chubr_version");
    minecraft_pro.remove("special data");

    map<std::string, IniSection> o = minecraft_pro.get_full();
    return 0;
}
