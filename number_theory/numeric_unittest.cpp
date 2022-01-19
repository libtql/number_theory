#include <stdint.h>

#include <cmath>
#include <limits>
#include <random>

#include <gtest/gtest.h>

#include "number_theory/numeric.h"

namespace tql {
namespace number_theory {

template <class T>
void test_exgcd(T a, T b) {
  auto [x, y] = exgcd(a, b);
  if (a != 0 && b != 0) {
    // Bézout's identity
    ASSERT_LE(std::abs(x), std::abs(b));
    ASSERT_LE(std::abs(y), std::abs(a));
  }
  ASSERT_EQ(x * a + y * b, gcd(a, b));
}

TEST(NumericTest, ExGCD) {
  for (int a = -10; a <= 100; ++a)
    for (int b = -100; b <= 10; ++b) {
      test_exgcd<int8_t>(a, b);
      test_exgcd<int16_t>(a, b);
      test_exgcd<int32_t>(a, b);
      test_exgcd<int64_t>(a, b);
    }
  // test large numbers
  std::default_random_engine rand_engine(
      testing::UnitTest::GetInstance()->random_seed());
  std::uniform_int_distribution<int64_t> rand_gen(
      std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
  for (int i = 0; i < 1000; ++i) {
    int64_t a = rand_gen(rand_engine);
    int64_t b = rand_gen(rand_engine);
    test_exgcd(a, b);
  }
}

}  // namespace number_theory
}  // namespace tql
