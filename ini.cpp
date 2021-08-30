//
// Created by uroborosq on 22.07.2021.
//

#include "ini.h"
#include <fstream>
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

void remove_extra_symbols(std::string& str)
{
    unsigned long pointer = 0;
    for (unsigned long i = 0; i < str.size(); ++i)
    {
        if (str[i] != ' ' and str[i] != '\t')
        {
            pointer = i;
            break;
        }
    }

    str = str.substr(pointer);

    for (unsigned long i = str.size() - 1; i >= 0; --i)
    {
        if (str[i] != ' ' and str[i] != '\t' or i == 0)
        {
            pointer = i;
            break;
        }
    }

    str = str.substr(0, pointer + 1);

    if (!str.empty())
    {
        if (*str.begin() == '"' and *(str.end() - 1) == '"')
            str = str.substr(1, str.size() - 2);
    }

}


IniFile::IniFile(const std::string& input) {
    open(input);
}

IniFile::~IniFile() {
}

int IniFile::open(const std::string& input)
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
//                    for (unsigned long i = 1 ; i < tmp.size() - 1; i++)
//                        name.push_back(tmp[i]);
                    name = tmp.substr(1, tmp.size() - 2); // ?
                    remove_extra_symbols(name);
                    push_back(IniSection(path, name, line_ending));
                    bool first_iter = true;
                    while(!file.eof() and !is_section(tmp) or first_iter) //parsing parameters
                    {
                        first_iter = false;
                        readline(file, tmp);
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

                                unsigned long pos = tmp.find('=');

                                parameter_name = tmp.substr(0, pos);
                                parameter_value = tmp.substr(pos + 1, tmp.size() - pos);

                                remove_extra_symbols(parameter_name);
                                remove_extra_symbols(parameter_value);

                                std::find(begin(), end(), name)->insert(make_pair(parameter_name, parameter_value));
                            }
                        }
                        //readline(file, tmp);
                    }
                }
                else
                {
                    readline(file, tmp);
                }
            }
        }
    }
    else
    {
        return -1;
//        throw std::runtime_error("ini_parser: No such file: " + input);
    }

    file.close();
    return 0;
}


std::vector<IniSection>::iterator IniFile::get(const std::string& key) {
    return find(begin(), end(), key);
}


std::vector<IniSection>::iterator IniFile::add(const std::string& section) {
    if(find(begin(), end(), section) == end())
    {
        std::fstream file;
        file.open(path.c_str(), std::ios_base::out | std::ios_base::app);
        writeline(file, "", line_ending);
        writeline(file, '[' + section + ']', line_ending);
        file.close();

        push_back(IniSection(path, section, line_ending));
    }
    return find(begin(), end(), section);
}

void IniFile::remove(const std::string& name) {
    if (find(begin(), end(), name) != end()) {
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

        erase(find(begin(), end(), name));
    }
}



IniSection::IniSection(const std::string& _path, const std::string& _name, const std::string& _line_ending) {
    path = _path;
    name = _name;
    line_ending = _line_ending;
}

IniSection::~IniSection() {
 
}

std::map<std::string, std::string>::iterator IniSection::get(const std::string& key) {
    return find(key);
}

void IniSection::add(const std::string& section, const std::string& parameter) {
    if(find(section) == end()) {
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

        erase(parameter);
    }
}

void IniSection::remove(const std::string& parameter) {
    if (find(parameter) != end()) {
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

        erase(parameter);
    }
}

bool IniSection::operator==(const IniSection &p) {
    if (name == p.name and path == p.path)
        return true;
    return false;
}

bool IniSection::operator==(const std::string &p) {
    if (name == p)
        return true;
    return false;
}
