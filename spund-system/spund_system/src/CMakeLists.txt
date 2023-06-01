cmake_minimum_required(VERSION 3.22)

project(
    Spund-System
    VERSION 3.0
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 23)

# add_library(ArduinoJson ArduinoJson.cpp ArduinoJson.hpp)

add_executable(spund main.cpp)