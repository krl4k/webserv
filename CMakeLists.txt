cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(/ /includes /srcs)
link_directories(/ /includes /srcs)

add_executable(webserv
        main.cpp
        srcs/WebServer.cpp
        srcs/Client.cpp
        srcs/Location.cpp
        srcs/Server.cpp
        srcs/Parser.cpp
        srcs/HttpRequest.cpp
        srcs/HttpResponse.cpp
        #        includes/WebServer.hpp
        #        includes/Client.hpp
        #        includes/Location.hpp
        #        includes/Server.hpp
        #        includes/Parser.hpp
        )
