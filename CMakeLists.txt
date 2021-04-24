cmake_minimum_required(VERSION 3.17)
project(webserv1)

set(CMAKE_CXX_STANDARD 11)

include_directories(/ /includes /srcs)
link_directories(/ /includes /srcs)

add_executable(webserv1
        main.cpp
        srcs/WebServer.cpp
        srcs/Client.cpp
        srcs/Location.cpp
        srcs/Server.cpp
        srcs/Parser.cpp
        srcs/HttpRequest.cpp
        srcs/HttpResponse.cpp
        srcs/ChunkedRequest.cpp
        srcs/CGI.cpp
        )

add_executable(test_cpp
        test_scripts/testCpp/testOpenDir.cpp)
