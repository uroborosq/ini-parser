//
// Created by uroborosq on 22.07.2021.
//

#ifndef INI_PARSER_INI_H
#define INI_PARSER_INI_H
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <vector>

class IniSection
{
private:
    std::map<std::string, std::string> parameters;
    std::string path;
    std::string name;
    std::string line_ending;
public:

    //default constructor default

    explicit IniSection(const std::string&, const std::string&, const std::string&);

    //copy constructor default

    //operator= default

    bool operator==(const IniSection& p);

    bool operator==(const std::string& p);

    ~IniSection();

    void insert(const std::string&, const std::string&);

    std::map<std::string, std::string>::iterator get(const std::string&);

    void add(const std::string&, const std::string&);

    void remove(const std::string&);

};

class IniFile
{
private:
    std::vector<IniSection> sections;
    std::string path;
    std::string line_ending;

public:
    IniFile(){}

    explicit IniFile(const std::string&);

    ~IniFile();

    void open(const std::string&);

    std::vector<IniSection>::iterator get(const std::string&);

    std::vector<IniSection>::iterator begin();

    std::vector<IniSection>::iterator end();

    std::vector<IniSection> & get_full();

    std::vector<IniSection>::iterator add(const std::string&);

    void remove(const std::string&);

};



#endif //INI_PARSER_INI_H
