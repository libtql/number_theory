#include <gtest/gtest.h>

#include "number_theory/numeric.h"

using namespace tql::number_theory;

TEST(NumericTest, Exgcd) {
  for (int a = 0; a <= 100; ++a)
    for (int b = 0; b <= 100; ++b) {
      auto [x, y] = exgcd(a, b);
      ASSERT_EQ(x * a + y * b, gcd(a, b));
    }
}
