#
# Copyright note: Redistribution and use in source, with or without modification, are permitted.
# 
# Created: August 2019
# 
# @author:	Andreas Richert
# @author:	Marco Dierschke
# SICK AG, Waldkirch
# email: TechSupport0905@sick.de

project(sick_visionary_cpp_shared)
cmake_minimum_required(VERSION 2.8)

### COMPILER FLAGS ###
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11")

### BUILD ###
aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_include_directories(${PROJECT_NAME}
    PUBLIC
      include
      .)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()
