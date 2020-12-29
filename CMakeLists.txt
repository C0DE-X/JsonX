cmake_minimum_required(VERSION 3.9)

option(BUILD_SAMPLE "Enable building samples" ON)

project(jsonx LANGUAGES CXX)

add_library(jsonx SHARED
    src/JsonObject.cpp
    src/JsonX.cpp
    src/Parser.cpp
    include/jsonx/JsonObject.h
    include/jsonx/JsonX.h
    src/Parser.h
)

set_target_properties(jsonx PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY "bin/"
    RUNTIME_OUTPUT_DIRECTORY "bin/"
    LIBRARY_OUTPUT_DIRECTORY "lib/"
    )

target_include_directories(jsonx PRIVATE src)
target_include_directories(jsonx PUBLIC include)

if(BUILD_SAMPLE)
    add_subdirectory(sample)
endif()