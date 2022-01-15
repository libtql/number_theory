# Test numeric.h
add_executable(numeric_unittest numeric_unittest.cpp)
target_link_libraries(numeric_unittest ${GTEST_LIBRARIES} GTest::Main)
add_test(NAME NumericTest COMMAND numeric_unittest)
