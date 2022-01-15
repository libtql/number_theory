cmake_minimum_required(VERSION 3.14)
project(tqlib)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)

# Find GoogleTest
find_package(GTest REQUIRED)

enable_testing()

# Add path to tqlib headers
include_directories(BEFORE ${PROJECT_SOURCE_DIR})
# Add path to GoogleTest headers
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(number_theory)
