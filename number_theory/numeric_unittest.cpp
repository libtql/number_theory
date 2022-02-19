#include <stdint.h>

#include <cmath>
#include <limits>
#include <numeric>
#include <random>

#include <gtest/gtest.h>

#include "number_theory/numeric.h"

namespace tql {
namespace number_theory {

template <typename T>
void test_exgcd(T a, T b) {
  auto [x, y] = exgcd(a, b);
  if (a != 0 && b != 0) {
    // Bézout's identity
    ASSERT_LE(unsigned_abs(x), unsigned_abs(b));
    ASSERT_LE(unsigned_abs(y), unsigned_abs(a));
  }
  ASSERT_EQ(x * a + y * b, std::gcd(a, b));
}

TEST(NumericTest, ExGCD) {
  // test small numbers
  for (int a = -10; a <= 100; ++a)
    for (int b = -100; b <= 10; ++b) {
      test_exgcd<int8_t>(a, b);
      test_exgcd<int16_t>(a, b);
      test_exgcd<int32_t>(a, b);
      test_exgcd<int64_t>(a, b);
      if (a >= 0 && b >= 0) {
        test_exgcd<uint8_t>(a, b);
        test_exgcd<uint16_t>(a, b);
        test_exgcd<uint32_t>(a, b);
        test_exgcd<uint64_t>(a, b);
      }
    }

  // test large numbers
  std::default_random_engine rand_engine(
      testing::UnitTest::GetInstance()->random_seed());
  std::uniform_int_distribution<int64_t> rand_gen(
      std::numeric_limits<int64_t>::min() + 1,
      std::numeric_limits<int64_t>::max());
  for (int i = 0; i < 1000; ++i) {
    int64_t a = rand_gen(rand_engine);
    int64_t b = rand_gen(rand_engine);
    test_exgcd<int64_t>(a, b);
    test_exgcd<uint64_t>(a, b);
  }

  // test overflow
  test_exgcd(std::numeric_limits<int>::min() + 1,
             std::numeric_limits<int>::max());
  test_exgcd(std::numeric_limits<int>::max(),
             std::numeric_limits<int>::min() + 1);
  test_exgcd(std::numeric_limits<int>::max(), 1);
  test_exgcd(std::numeric_limits<int>::min() + 1, 1);
  test_exgcd(1, std::numeric_limits<int>::max());
  test_exgcd(1, std::numeric_limits<int>::min() + 1);
}

TEST(NumericTest, Pow) {
  // test integer exponents
  EXPECT_EQ(pow(int8_t(1), -1), 1);
  EXPECT_EQ(pow(int16_t(2), 0), 1);
  EXPECT_EQ(pow(int32_t(5), 10), 9765625);
  EXPECT_EQ(pow(int64_t(10), -2), 0);
  EXPECT_EQ(pow(uint8_t(1), -1), 1);
  EXPECT_EQ(pow(uint16_t(2), 0), 1);
  EXPECT_EQ(pow(uint32_t(5), 10), 9765625);
  EXPECT_EQ(pow(uint64_t(10), -2), 0);
  EXPECT_FLOAT_EQ(pow(2.2f, 3), 10.648f);
  EXPECT_DOUBLE_EQ(pow(1e-3, -10), 1e30);

  // test floating point exponents
  EXPECT_FLOAT_EQ(pow(3.0f, -0.5f), 1.0f / std::sqrt(3.0f));
  EXPECT_DOUBLE_EQ(pow(2, 1.1), std::pow(2, 1.1));
}

template <typename T>
void test_integer_root() {
  // test small numbers
  EXPECT_EQ(integer_root(T(120), 2), 10);
  EXPECT_EQ(integer_root(T(125), 3), 5);
  EXPECT_EQ(integer_root(T(0), 1), 0);
  if (std::numeric_limits<T>::is_signed) {
    EXPECT_EQ(integer_root(T(-125), 3), -5);
    EXPECT_EQ(integer_root(T(-30), 3), -3);
  }

  // test exceptions
  EXPECT_THROW(integer_root(T(2), 0), std::domain_error);
  if (std::numeric_limits<T>::is_signed)
    EXPECT_THROW(integer_root(T(-4), 2), std::domain_error);
}

TEST(NumericTest, IntegerRoot) {
  test_integer_root<int8_t>();
  test_integer_root<int16_t>();
  test_integer_root<int32_t>();
  test_integer_root<int64_t>();
  test_integer_root<uint8_t>();
  test_integer_root<uint16_t>();
  test_integer_root<uint32_t>();
  test_integer_root<uint64_t>();

  // test overflow
  EXPECT_EQ(integer_root(std::numeric_limits<int32_t>::max(), 2), 46340);
  EXPECT_EQ(integer_root(std::numeric_limits<uint32_t>::max(), 3), 1625);
  EXPECT_EQ(integer_root(std::numeric_limits<int64_t>::min(), 5), -6208);
  EXPECT_EQ(integer_root(std::numeric_limits<uint64_t>::max(), 10), 84);
}

}  // namespace number_theory
}  // namespace tql
