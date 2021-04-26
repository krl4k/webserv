cmake_minimum_required(VERSION 3.17)
project(webserv1)

#set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD 98)
add_compile_options( -Wall -Wextra -Werror -Wconversion -Wno-unused -Wunused-value  -Wunused-variable -Wunused-macros -Wunused-function  -Wunused-label)
add_compile_options(-Wunused-local-typedefs -Wunused-const-variable)

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
        srcs/AutoIndexGenerate.cpp
        srcs/base64.cpp
        srcs/CGI.cpp

#        tmp/testfiles/testCpp.cpp
        )



add_executable(test_cpp
        srcs/AutoIndexGenerate.cpp
        )
