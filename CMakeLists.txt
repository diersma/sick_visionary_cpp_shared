#
# Copyright note: Redistribution and use in source, with or without modification, are permitted.
# 
# Updated: January 2020
# 
# Author: Andreas Richert
# Author: Marco Dierschke
#
# SICK AG, Waldkirch
# email: TechSupport0905@sick.de

cmake_minimum_required(VERSION 3.0.2)

project(sick_visionary_cpp_shared
        VERSION 0.1.0
        LANGUAGES CXX)

### COMPILER FLAGS ###
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

### BUILD ###
aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(${PROJECT_NAME}
    PUBLIC
      include
      .)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()
