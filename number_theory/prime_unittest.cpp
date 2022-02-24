#include <stddef.h>

#include <limits>
#include <numeric>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include "number_theory/prime.h"

namespace tql::number_theory {

template <typename T>
void test_sieve_primes() {
  std::unordered_set<T> primes{2,  3,  5,  7,  11, 13, 17, 19, 23,
                               29, 31, 37, 41, 43, 47, 53, 59, 61,
                               67, 71, 73, 79, 83, 89, 97};
  Sieve sieve(T(97));
  for (T i = 0; i <= 97; ++i)
    EXPECT_EQ(sieve.is_prime(i), primes.contains(i));
  if (std::numeric_limits<T>::is_signed)
    EXPECT_FALSE(sieve.is_prime(-5));
  EXPECT_THROW(sieve.is_prime(100), std::out_of_range);
}

TEST(SieveTest, Primes) {
  test_sieve_primes<int8_t>();
  test_sieve_primes<int16_t>();
  test_sieve_primes<int32_t>();
  test_sieve_primes<int64_t>();
  test_sieve_primes<uint8_t>();
  test_sieve_primes<uint16_t>();
  test_sieve_primes<uint32_t>();
  test_sieve_primes<uint64_t>();
}

template <typename T>
void test_euler_sieve_primes() {
  EulerSieve sieve{T(97)};
  EXPECT_EQ(sieve.get_limit(), 97);
  std::vector<T> primes = sieve.primes();
  std::vector<T> expected{2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37, 41,
                          43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
  ASSERT_EQ(primes.size(), expected.size());
  for (size_t i = 0; i < primes.size(); ++i)
    EXPECT_EQ(primes[i], expected[i]);
}

TEST(EulerSieveTest, Primes) {
  test_euler_sieve_primes<int8_t>();
  test_euler_sieve_primes<int16_t>();
  test_euler_sieve_primes<int32_t>();
  test_euler_sieve_primes<int64_t>();
  test_euler_sieve_primes<uint8_t>();
  test_euler_sieve_primes<uint16_t>();
  test_euler_sieve_primes<uint32_t>();
  test_euler_sieve_primes<uint64_t>();
}

template <typename T>
void test_min_prime_factor() {
  EulerSieve sieve{T(100)};
  EXPECT_EQ(sieve.min_prime_factor(15), 3);
  EXPECT_EQ(sieve.min_prime_factor(23), 23);
  if (std::numeric_limits<T>::is_signed) {
    EXPECT_EQ(sieve.min_prime_factor(-15), 3);
    EXPECT_EQ(sieve.min_prime_factor(-23), 23);
  }
  EXPECT_THROW(sieve.min_prime_factor(0), std::domain_error);
  EXPECT_THROW(sieve.min_prime_factor(1), std::domain_error);
  EXPECT_THROW(sieve.min_prime_factor(101), std::out_of_range);
}

TEST(EulerSieveTest, MinPrimeFactor) {
  test_min_prime_factor<int8_t>();
  test_min_prime_factor<int16_t>();
  test_min_prime_factor<int32_t>();
  test_min_prime_factor<int64_t>();
  test_min_prime_factor<uint8_t>();
  test_min_prime_factor<uint16_t>();
  test_min_prime_factor<uint32_t>();
  test_min_prime_factor<uint64_t>();
}

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
