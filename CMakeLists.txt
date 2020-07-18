cmake_minimum_required(VERSION 3.9)

project(jsonx LANGUAGES CXX)

#add_library(jsonx SHARED
#)

add_executable(jsonx
    src/main.cpp
    src/JsonObject.cpp
    src/JsonX.cpp
    include/jsonx/JsonObject.h
    include/jsonx/JsonX.h
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