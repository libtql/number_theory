#include <stdint.h>

#include <numeric>
#include <set>
#include <utility>

#include <gtest/gtest.h>

#include "number_theory/prime.h"

namespace tql::number_theory {

template <typename T>
void test_coprime_pairs() {
  T n = 100;
  auto pairs = coprime_pairs(n);
  for (auto [x, y] : pairs) {
    ASSERT_GE(n, x);
    ASSERT_GE(x, y);
    ASSERT_GE(y, T(0));
  }
  std::set<std::pair<T, T>> pairs_set(pairs.begin(), pairs.end());
  for (T x = 0; x <= n; ++x)
    for (T y = 0; y <= x; ++y) {
      bool is_coprime = std::gcd(x, y) == 1;
      EXPECT_EQ(pairs_set.contains(std::pair(x, y)), is_coprime);
    }
}

TEST(CoprimeTest, CoprimePairs) {
  test_coprime_pairs<int8_t>();
  test_coprime_pairs<int16_t>();
  test_coprime_pairs<int32_t>();
  test_coprime_pairs<int64_t>();
  test_coprime_pairs<uint8_t>();
  test_coprime_pairs<uint16_t>();
  test_coprime_pairs<uint32_t>();
  test_coprime_pairs<uint64_t>();
}

}  // namespace tql::number_theory
