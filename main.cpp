#include <iostream>
#include "ini.h"
using namespace std;

int main() {
    IniFile minecraft_pro;
    minecraft_pro.open("nvClient5D8A8410.sms");
    vector<IniSection> tag = minecraft_pro.get_full();
    int i;
    for ( i=0;i<tag.size();i++){
        cout << "[" << tag[i].getName() << "]" << endl;
        std::map<std::string, std::string> tmp = tag[i].getParameters();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); ++it){
            std::cout << it->first << "="<<it->second<<std::endl;
        }
    }
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


    return 0;
}
