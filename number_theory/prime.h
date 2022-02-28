#ifndef NUMBER_THEORY_PRIME_H_
#define NUMBER_THEORY_PRIME_H_

#include <stddef.h>
#include <stdint.h>

#include <limits>
#include <utility>
#include <vector>

#include "number_theory/utility.h"

// This file contains functions related to prime numbers.

namespace tql {
namespace number_theory {

// Generates all coprime pairs of integers under |num_limit| (inclusive).
// Each pair (x, y) in the results should satisfy num_limit >= x >= y >= 0.
template <typename T>
std::vector<std::pair<T, T>> coprime_pairs(const T &num_limit) {
  static_assert(std::numeric_limits<T>::is_integer,
                "coprime_pairs argument |num_limit| must be an integer.");
  if (num_limit <= 0)
    return {};
  uint64_t num_limit_u64 = numeric_cast<uint64_t>(num_limit);
  std::vector<std::pair<T, T>> pairs;

  // Append pair (x, y) to |pairs| if it is under the limit.
  // The caller should make sure x >= y.
  auto add_pair = [&pairs, num_limit_u64](uint64_t x, uint64_t y) {
    if (x <= num_limit_u64) {
      pairs.emplace_back(static_cast<T>(x), static_cast<T>(y));
    }
  };

  // The algorithm comes from
  // https://web.archive.org/web/20220225121243/https://en.wikipedia.org/wiki/Coprime_integers
  add_pair(2, 1);
  add_pair(3, 1);
  size_t visited = 0;
  while (visited < pairs.size()) {
    const auto &current = pairs[visited++];
    uint64_t x = static_cast<uint64_t>(current.first);
    uint64_t y = static_cast<uint64_t>(current.second);
    add_pair(2 * x - y, x);
    add_pair(2 * x + y, x);
    add_pair(x + 2 * y, y);
  }
  add_pair(1, 0);
  add_pair(1, 1);
  return pairs;
}

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

using number_theory::is_prime;

}  // namespace tql

#endif  // NUMBER_THEORY_PRIME_H_
