cmake_minimum_required(VERSION 3.10)
project(tqlib)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)

# Always build tests with debug info
set(CMAKE_BUILD_TYPE Debug)

# Find GoogleTest
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

# Add path to tqlib headers
include_directories(BEFORE ${PROJECT_SOURCE_DIR})
# Add path to GoogleTest headers
include_directories(${GTEST_INCLUDE_DIRS})

# This function adds a new unittest to the source file list.
function(add_unittest)
  foreach(Source IN LISTS ARGN)
    list(APPEND UNITTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${Source}")
  endforeach()
  set(UNITTEST_SOURCES ${UNITTEST_SOURCES} PARENT_SCOPE)
endfunction()

# This macro exposes UNITTEST_SOURCES variable to parent scope, so we can access
# the files added from subdirectories. 
macro(expose_unittest)
  set(UNITTEST_SOURCES ${UNITTEST_SOURCES} PARENT_SCOPE)
endmacro()


# Add tests from subdirectories
add_subdirectory(number_theory)
# ... more


# Build the unittest
add_executable(unittest "${UNITTEST_SOURCES}")
target_link_libraries(unittest ${GTEST_LIBRARIES} GTest::Main)

# Add tests to CTest automatically 
gtest_discover_tests(unittest)
