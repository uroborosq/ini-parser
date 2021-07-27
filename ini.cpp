//
// Created by uroborosq on 22.07.2021.
//

#include "ini.h"
#include <fstream>
#include <vector>

IniFile::IniFile(const std::string& input) {
    open(input);
}

IniFile::~IniFile() {
    sections.clear();
}

void IniFile::open(const std::string& input)
{
    std::fstream file;
    file.open(input.c_str(), std::fstream::in | std::fstream::out);
    path = input;
    std::string tmp;

    if(file)
    {
        getline(file, tmp);

        while(!file.eof())
        {
            if (!tmp.empty())
            {
                if (*tmp.begin() == ';') //its comment, skip
                {
                    getline(file, tmp);
                    continue;
                }


                else if(*tmp.begin() == '[' && *(tmp.end() - 1) == ']') // new section
                {
                    std::string name;
                    for (unsigned long i = 1; i < tmp.size() - 1; i++)
                        name.push_back(tmp[i]);

                    sections.insert(std::pair<std::string, IniSection>(name, IniSection(path, name)));
                    getline(file, tmp);
                    while(!file.eof() && tmp.find('[') == std::string::npos) //parsing parameters
                    {

                        if(!tmp.empty())
                        {
                            if(*tmp.begin() == ';')
                            {
                                getline(file, tmp);
                                continue;
                            }
                            else if(tmp.find('=') != std::string::npos)
                            {
                                std::string parameter_name, parameter_value;
                                for (unsigned long i = 0; i < tmp.find('='); i++)
                                    parameter_name.push_back(tmp[i]);
                                for (unsigned long i = tmp.find('=') + 1; i < tmp.size(); i++)
                                    parameter_value.push_back(tmp[i]);

                                sections[name].insert(parameter_name, parameter_value);
                            }
                        }
                        getline(file, tmp);
                    }
                }
            }
        }
    }
    else
    {
        throw std::runtime_error("ini_parser: No such file: " + input);
    }

    file.close();
}


std::map<std::string, IniSection>::iterator IniFile::get(const std::string& key) {
    return sections.find(key);
}

std::map<std::string, IniSection>::iterator IniFile::begin() {
    return sections.begin();
}

std::map<std::string, IniSection>::iterator IniFile::end() {
    return sections.end();
}

std::map<std::string, IniSection> &IniFile::get_full() {
    return sections;
}

void IniFile::add_section(const std::string& section_name) {

}

void IniFile::add_parameter(const std::string& key, const std::string& value) {

}

void IniFile::add(const std::string& section) {
    if(sections.find(section) == sections.end())
    {
        std::ofstream file;
        file.open(path.c_str(), std::ios_base::out | std::ios_base::app);
        file << std::endl;
        file.write(('[' + section + "]").c_str(), section.size() + 2);
        file << std::endl;
        file.close();

        sections.insert(std::pair<std::string, IniSection>(section, IniSection(path, section)));
    }
}

void IniFile::remove(const std::string& name) {
    std::fstream file;
    file.open(path.c_str(), std::ios_base::in);
    std::string tmp;

    std::vector<std::string> all_file;
    while(!file.eof())
    {
        getline(file, tmp);

        if (tmp == '[' + name + ']')
        {
            getline(file, tmp);
            while(!file.eof() and *tmp.begin() != '[' and *(tmp.end()-1) != ']')
            {
                if (*tmp.begin() == ';')
                    all_file.push_back(tmp);
                getline(file, tmp);
            }
            all_file.push_back(tmp);
        }
        else
            all_file.push_back(tmp);
    }
    file.close();
    file.open(path.c_str(),  std::ios_base::out);
    unsigned long size = all_file.size();
    for(unsigned long i = 0; i < size; i++)
    {
        file << all_file[i] << std::endl;
    }
    file.close();

    sections.erase(name);
}



IniSection::IniSection(const std::string& _path, const std::string& _name) {
    path = _path;
    name = _name;
}

IniSection::~IniSection() {
    parameters.clear();
}

void IniSection::insert(const std::string& key, const std::string& value)  {
    parameters.insert(std::pair<std::string, std::string>(key, value));
}

std::map<std::string, std::string>::iterator IniSection::get(const std::string& key) {
    return parameters.find(key);
}

void IniSection::add(const std::string& section, const std::string& parameter) {
    std::fstream file;
    file.open(path.c_str(), std::ios_base::out | std::ios_base::in);

    std::string tmp;
    getline(file, tmp);

    while(!file.eof() && tmp != '[' + name + ']')
    {
        getline(file, tmp);
    }

    file << std::endl << section + '=' + parameter << std::endl;
    file.close();

}

void IniSection::remove(const std::string& parameter) {
    std::fstream file;
    file.open(path.c_str(), std::ios_base::in);
    std::string tmp;

    std::vector<std::string> all_file;
    while(!file.eof())
    {
        getline(file, tmp);

        if (tmp == '[' + name + ']')
        {
            all_file.push_back(tmp);
            while(!file.eof())
            {
                getline(file, tmp);
                if (tmp.find(parameter) != std::string::npos)
                    break;
                all_file.push_back(tmp);

            }

        }
        else
            all_file.push_back(tmp);
    }
    file.close();
    file.open(path.c_str(),  std::ios_base::out);
    unsigned long size = all_file.size();
    for(unsigned long i = 0; i < size; i++)
    {
        file << all_file[i] << std::endl;
    }
    file.close();

    parameters.erase(parameter);
}
