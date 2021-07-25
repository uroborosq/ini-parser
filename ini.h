//
// Created by uroborosq on 22.07.2021.
//

#ifndef INI_PARSER_INI_H
#define INI_PARSER_INI_H
#include <fstream>
#include <string>
#include <map>
#include <iterator>


class IniParameter
{
private:
    std::string name;
    std::string value;
public:
    IniParameter();

    IniParameter(std::string);

    IniParameter(std::string, std::string);

    IniParameter(const IniParameter&);

    IniParameter& operator=(const IniParameter&);

    ~IniParameter();


};


class IniSection
{
private:
    std::string name;
    std::map<std::string, IniParameter> parameters;
public:
    IniSection();

    IniSection(std::string);

    IniSection(const IniSection& p);

    IniSection& operator=(const IniSection& p);

    ~IniSection();

 };

class IniFile
{
private:
    std::string path;
    std::map<std::string, IniSection> sections;


public:
    IniFile();

    explicit IniFile(std::string);

    IniFile(const IniFile& p);

    ~IniFile();

    IniFile& operator=(const IniFile& p);

    IniFile open(std::string);

    IniFile open(FILE*);

    std::map<std::string, IniSection>::iterator get(std::string);

    std::map<std::string, IniSection>::iterator begin();

    std::map<std::string, IniSection>::iterator end();


};



#endif //INI_PARSER_INI_H
