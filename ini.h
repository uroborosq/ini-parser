//
// Created by uroborosq on 22.07.2021.
//

#ifndef INI_PARSER_INI_H
#define INI_PARSER_INI_H

#include <string>
#include <map>
#include <vector>

class IniSection : private std::map<std::string, std::string>
{
private:
    
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

    virtual ~IniSection();

    

    std::map<std::string, std::string>::iterator get(const std::string&);

    void add(const std::string&, const std::string&);

    void remove(const std::string&);

    std::string getName(){return name;}

    friend class IniFile;
 
};

class IniFile : private std::vector<IniSection>
{
private:
    std::string path;
    std::string line_ending;

public:
    IniFile(){}

    explicit IniFile(const std::string&);

    virtual ~IniFile();

    int open(const std::string&);

    std::vector<IniSection>::iterator get(const std::string&);

    std::vector<IniSection>::iterator add(const std::string&);

    void remove(const std::string&);

};



#endif //INI_PARSER_INI_H
