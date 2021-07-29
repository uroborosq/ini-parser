//
// Created by uroborosq on 22.07.2021.
//

#include "ini.h"
#include <algorithm>


bool is_section(std::string str)
{
    if(str.size() >= 2)
    {
        if (*str.begin() == '[' and *(str.end() - 1) == ']')
            return true;
        if (str.size() >= 3)
        {
            if (*str.begin() == '[' and *(str.end() - 2) == ']' and *(str.end() - 1) == '\r')
                return true;
        }
    }
    return false;
}

std::string readline(std::fstream& file, std::string& str)
{
    getline(file, str);
    if (!str.empty())
    {
        if (*(str.end() - 1) == '\r')
        {
            str = str.substr(0, str.size() - 1);
            return "crlf";
        }
    }
    return "lf";
}

void writeline(std::fstream& file, std::string str, const std::string& line_ending)
{
    if (line_ending == "lf")
    {
        file << str << '\n';
    }
    else
    {
        file << str << "\r\n";
    }
}




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
        line_ending = readline(file, tmp);

        while(!file.eof())
        {
            if (!tmp.empty())
            {
                if (*tmp.begin() == ';') //its comment, skip
                {
                    readline(file, tmp);
                    continue;
                }


                else if(is_section(tmp)) // new section
                {
                    std::string name;
                    for (unsigned long i = 1 ; i < tmp.size() - 1; i++)
                        name.push_back(tmp[i]);
                    sections.push_back(IniSection(path, name, line_ending));
                    readline(file, tmp);
                    while(!file.eof() && !is_section(tmp)) //parsing parameters
                    {

                        if(!tmp.empty())
                        {
                            if(*tmp.begin() == ';')
                            {
                                readline(file, tmp);
                                continue;
                            }
                            else if(tmp.find('=') != std::string::npos)
                            {
                                std::string parameter_name, parameter_value;
                                for (unsigned long i = 0; i < tmp.find('='); i++)
                                    parameter_name.push_back(tmp[i]);
                                for (unsigned long i = tmp.find('=') + 1; i < tmp.size(); i++)
                                    parameter_value.push_back(tmp[i]);

                                find(sections.begin(), sections.end(), name)->insert(parameter_name, parameter_value);
                            }
                        }
                        readline(file, tmp);
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


std::vector<IniSection>::iterator IniFile::get(const std::string& key) {
    return find(sections.begin(), sections.end(), key);
}

std::vector<IniSection>::iterator IniFile::begin() {
    return sections.begin();
}

std::vector<IniSection>::iterator IniFile::end() {
    return sections.end();
}

std::vector<IniSection> &IniFile::get_full() {
    return sections;
}

std::vector<IniSection>::iterator IniFile::add(const std::string& section) {
    if(find(sections.begin(), sections.end(), section) == sections.end())
    {
        std::fstream file;
        file.open(path.c_str(), std::ios_base::out | std::ios_base::app);
        writeline(file, "", line_ending);
        writeline(file, '[' + section + ']', line_ending);
        file.close();

        sections.push_back(IniSection(path, section, line_ending));
    }
    return find(sections.begin(), sections.end(), section);
}

void IniFile::remove(const std::string& name) {
    if (find(sections.begin(), sections.end(), name) != sections.end()) {
        std::fstream file;
        file.open(path.c_str(), std::ios_base::in);
        std::string tmp;

        std::vector<std::string> all_file;
        while (!file.eof())
        {
            readline(file, tmp);

            if (tmp == '[' + name + ']')
            {
                readline(file, tmp);
                while (!file.eof() and !is_section(tmp))
                {
                    if (*tmp.begin() == ';')
                        all_file.push_back(tmp);
                    readline(file, tmp);
                }
                all_file.push_back(tmp);
            } else
                all_file.push_back(tmp);
        }
        file.close();
        file.open(path.c_str(), std::ios_base::out);
        unsigned long size = all_file.size();
        for (unsigned long i = 0; i < size; i++) {
            writeline(file, all_file[i], line_ending);
        }
        file.close();

        sections.erase(find(sections.begin(), sections.end(), name));
    }
}



IniSection::IniSection(const std::string& _path, const std::string& _name, const std::string& _line_ending) {
    path = _path;
    name = _name;
    line_ending = _line_ending;
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
    if(parameters.find(section) == parameters.end()) {
        std::fstream file;
        file.open(path.c_str(), std::ios_base::in);
        std::string tmp;

        std::vector<std::string> all_file;
        while (!file.eof())
            readline(file, tmp), all_file.push_back(tmp);

        file.close();
        file.open(path.c_str(), std::ios_base::out);
        unsigned long size = all_file.size() + 1;
        for (unsigned long i = 0; i < size; i++) {
            if (all_file[i] == '[' + name + ']') {
                writeline(file, all_file[i], line_ending);
                i++;
                while (i < size - 1 and is_section(all_file[i]))
                    writeline(file, all_file[i], line_ending), i++;
                writeline(file, section + '=' + parameter, line_ending);
            }
            writeline(file, all_file[i], line_ending);
        }
        file.close();

        parameters.erase(parameter);
    }
}

void IniSection::remove(const std::string& parameter) {
    if (parameters.find(parameter) != parameters.end()) {
        std::fstream file;
        file.open(path.c_str(), std::ios_base::in);
        std::string tmp;

        std::vector<std::string> all_file;
        while (!file.eof()) {
            readline(file, tmp);

            if (tmp == '[' + name + ']') {
                all_file.push_back(tmp);
                while (!file.eof()) {
                    readline(file, tmp);
                    if (tmp.find(parameter) != std::string::npos)
                        break;
                    all_file.push_back(tmp);

                }

            } else
                all_file.push_back(tmp);
        }
        file.close();
        file.open(path.c_str(), std::ios_base::out);
        unsigned long size = all_file.size();
        for (unsigned long i = 0; i < size; i++) {
            writeline(file, all_file[i], line_ending);
        }
        file.close();

        parameters.erase(parameter);
    }
}

bool IniSection::operator==(const IniSection &p) {
    if (parameters == p.parameters and name == p.name and path == p.path)
        return true;
    return false;
}

bool IniSection::operator==(const std::string &p) {
    if (name == p)
        return true;
    return false;
}
