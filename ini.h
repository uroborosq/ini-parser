//
// Created by uroborosq on 22.07.2021.
//

#ifndef INI_PARSER_INI_H
#define INI_PARSER_INI_H
#include <fstream>
#include <string>
#include <map>
#include <iterator>

class IniSection
{
private:
    std::map<std::string, std::string> parameters;
    std::string path;
    std::string name;
public:

    explicit IniSection(const std::string&, const std::string&);

    //copy constructor default

    //operator= default

    ~IniSection();

    void insert(const std::string&, const std::string&);

    std::map<std::string, std::string>::iterator get(const std::string&);

    void add(const std::string&, const std::string&);

    void remove(const std::string&);


    IniSection(){}
};

class IniFile
{
private:
    std::map<std::string, IniSection> sections;
    std::string path;

    void add_section(const std::string&);

    void add_parameter(const std::string&, const std::string&);

public:
    IniFile(){}

    explicit IniFile(const std::string&);

    ~IniFile();

    void open(const std::string&);

    std::map<std::string, IniSection>::iterator get(const std::string&);

    std::map<std::string, IniSection>::iterator begin();

    std::map<std::string, IniSection>::iterator end();

    std::map<std::string, IniSection>& get_full();

    void add(const std::string&);

    void remove(const std::string&);

};



#endif //INI_PARSER_INI_H
