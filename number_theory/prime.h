#ifndef NUMBER_THEORY_PRIME_H_
#define NUMBER_THEORY_PRIME_H_

#include <stddef.h>
#include <stdint.h>

#include <bit>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "number_theory/utility.h"

// This file contains functions related to prime numbers.

namespace tql {
namespace number_theory {

// Sieve of Eratosthenes.
// It decides whether a number is prime or not for the numbers up to a given
// (inclusive) limit.
template <typename T>
class Sieve {
  static_assert(std::numeric_limits<T>::is_integer,
                "Sieve must use integer types.");

 public:
  // The type of numbers used by the Sieve.
  using type = T;

  explicit Sieve(const T &num_limit)
      : num_limit_(num_limit),
        is_prime_(numeric_cast<size_t>(num_limit) + 1, true) {
    is_prime_[0] = false;
    is_prime_[1] = false;
    // Use uint64_t to avoid multiplication overflow. Due to the time and space
    // complexity of the algorithm, uint64_t is large enough.
    for (uint64_t i = 2; i * i <= num_limit_; ++i) {
      if (!is_prime_[i])
        continue;
      for (uint64_t j = i * i; j <= num_limit_; j += i) {
        is_prime_[j] = false;
      }
    }
  }

  Sieve(const Sieve &) = default;
  Sieve(Sieve &&) = default;
  Sieve &operator=(const Sieve &) = default;
  Sieve &operator=(Sieve &&) = default;

  // Returns whether |number| is prime or not.
  bool is_prime(const T &number) {
    if (number < 0)
      return false;
    if (number > num_limit_)
      throw std::out_of_range("The number exceeds the limit of Sieve.");
    return is_prime_[number];
  }

 private:
  // The maximum number (inclusive) we can hold.
  T num_limit_;
  std::vector<bool> is_prime_;
};

// Sieve of Euler.
// It finds all prime numbers under a certain limit.
// It also provides the factorizations of all numbers under the limit.
template <typename T>
class EulerSieve {
  static_assert(std::numeric_limits<T>::is_integer,
                "EulerSieve must use integer types.");

 public:
  // The type of numbers used by the Sieve.
  using type = T;

  // Constructs the sieve in linear time.
  explicit EulerSieve(const T &num_limit)
      : num_limit_(num_limit),
        min_prime_factor_(numeric_cast<size_t>(num_limit) + 1) {
    check_overflow();
    // Euler's Sieve algorithm
    for (T num = 2; num <= num_limit_; ++num) {
      if (min_prime_factor_[num] == 0) {
        primes_.push_back(num);
        min_prime_factor_[num] = num;
      }
      for (const T &prime : primes_) {
        if (prime > min_prime_factor_[num])
          break;
        MultType x = static_cast<MultType>(prime) * static_cast<MultType>(num);
        if (x > static_cast<MultType>(num_limit_))
          break;
        min_prime_factor_[x] = prime;
      }
    }
  }

  EulerSieve(const EulerSieve &other) = default;
  EulerSieve(EulerSieve &&other) = default;
  EulerSieve &operator=(const EulerSieve &other) = default;
  EulerSieve &operator=(EulerSieve &&other) = default;

  // Returns the maximum number (inclusive) we can hold.
  const T &get_limit() const { return num_limit_; }

  // Returns the list of primes.
  const std::vector<T> &primes() const { return primes_; }

  // Returns the minimum prime factor of the |number|.
  // Throws domain_error exception if minimum prime factor does not exist.
  T min_prime_factor(const T &number) const {
    auto abs_num = unsigned_abs(number);
    if (abs_num <= 1)
      throw std::domain_error("Minimum prime factor does not exist.");
    if (abs_num > static_cast<std::make_unsigned_t<T>>(num_limit_))
      throw std::out_of_range("The number exceeds the limit of Sieve.");
    return min_prime_factor_[abs_num];
  }

 private:
  // The maximum number (inclusive) we can hold.
  T num_limit_;
  // Minimum prime factors.
  std::vector<T> min_prime_factor_;
  // Prime numbers.
  std::vector<T> primes_;

  // Numbers are converted to MultType during multiplication to avoid overflow.
  // uint64_t is large enough due to the time and space complexity of the sieve
  // algorithm.
  using MultType = uint64_t;

  // Throws overflow_error exception if multiplication will overflow.
  void check_overflow() {
    size_t num_limit_width =
        std::bit_width(static_cast<std::make_unsigned_t<T>>(num_limit_));
    if (num_limit_width * 2 > std::numeric_limits<MultType>::digits)
      throw std::overflow_error(
          "Multiplication will overflow when sieving. "
          "Please use larger integer types.");
  }
};

// Tests whether |number| is prime or not.
//
// This overload is only available for numbers smaller than 2^16 since it
// performs poorly for large numbers.
template <typename T,
          std::enable_if_t<std::numeric_limits<T>::is_integer &&
                               std::numeric_limits<T>::digits <= 16,
                           bool> = true>
constexpr bool is_prime(const T &number) {
  // There are no prime numbers smaller than 2.
  if (number < 2)
    return false;
  for (int i = 2; i * i <= number; ++i) {
    if (number % i == 0)
      return false;
  }
  return true;
}

}  // namespace number_theory

using number_theory::EulerSieve;
using number_theory::Sieve;

using number_theory::is_prime;

}  // namespace tql

#endif  // NUMBER_THEORY_PRIME_H_
