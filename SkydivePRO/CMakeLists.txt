cmake_minimum_required(VERSION 3.10)
project(SkydivePRO)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find source files
file(GLOB SOURCES src/*.cpp)

# Add executable
add_executable(SkydivePRO ${SOURCES})
