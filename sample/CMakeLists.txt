cmake_minimum_required(VERSION 3.9)

project(sample LANGUAGES CXX)

add_executable(sample
    main.cpp
)

set_target_properties(sample PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY "bin/"
    RUNTIME_OUTPUT_DIRECTORY "bin/"
    LIBRARY_OUTPUT_DIRECTORY "lib/"
)

target_include_directories(sample PUBLIC ../include)
target_link_directories(sample PUBLIC ../build/lib)
target_link_libraries(sample jsonx)