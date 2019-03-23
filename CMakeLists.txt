cmake_minimum_required(VERSION 2.8.9)
project(boiler)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

file(GLOB SOURCES "src/*.cc")

 
add_subdirectory(src/nlohmann_json)
add_library(nlohmann ${SOURCES})
target_link_libraries(nlohmann PRIVATE nlohmann_json::nlohmann_json)
add_executable(boiler ${SOURCES})