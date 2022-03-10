cmake_minimum_required(VERSION 2.8) 
project(fairway)
            
FILE(GLOB SRCS *.cc)
add_executable(client ${SRCS})		