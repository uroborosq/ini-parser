cmake_minimum_required(VERSION 3.19)
project(ini_parser)

set(CMAKE_CXX_STANDARD 98)

add_executable(ini_parser main.cpp ini.cpp ini.h)