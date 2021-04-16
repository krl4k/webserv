cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(webserv
        CGI.cpp
        CGI.hpp)
