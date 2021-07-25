//
// Created by uroborosq on 22.07.2021.
//

#include "ini.h"
#include <fstream>

IniFile::IniFile() {

}

IniFile::IniFile(std::string) {

}

IniFile::IniFile(const IniFile &p) {

}

IniFile::~IniFile() {

}

IniFile &IniFile::operator=(const IniFile &p){

}

IniFile IniFile::open(std::string path) {
    std::fstream file = std::fstream().open(path, 'w')
    return IniFile();
}

IniFile IniFile::open(FILE *) {
    return
}

std::map<std::string, IniSection>::iterator IniFile::get(std::string key) {
    return sections.find(key)
}

std::map<std::string, IniSection>::iterator IniFile::begin() {
    return sections.begin()
}

std::map<std::string, IniSection>::iterator IniFile::end() {
    return sections.end();
}
