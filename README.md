# ini_parser
Simple library on C++98 to manage ini files. Supports LF and CRLF line-endings.
Removes tabs, spaces and brackets from sections, keys and values while parsing

## Installation
Just put ini.h and ini.cpp in the same comfortable for you place and make sure to include headers in your code
```C++
#include "ini.h"
```
## Usage

### Creating an object of file
To start parsing the ini-like file, you should create an IniFile objects.
You can initialize it and then specify path or use special constuctor
```C++

IniFile file1;
file1.open("/path/to/your/file1")

IniFile file2("/path/to/your/file2")
```

If opening of file is not succesful, function open will return -1

### IniFile
#### Get
Returns map iterator to section. If there is no such section, it return end iterator

#### Add
Create new section in the local map in RAM and in the file

### Remove
Remove section from file and local map

### IniSection

#### Get
Return value of parametr

#### Add
Add new parameter with value

#### Remove
Removes parameter with value :)
