#ifndef NUMBER_THEORY_NUMERIC_H_
#define NUMBER_THEORY_NUMERIC_H_

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <type_traits>
#include <utility>

#include "number_theory/utility.h"

// Common numeric functions.

namespace tql {
namespace number_theory {

using std::gcd;
using std::lcm;

// Extended Euclidean algorithm.
// Given two numbers |a| and |b|, returns a pair (x, y) satisfying
// x*a + y*b == gcd(a,b).
template <typename T>
constexpr std::pair<std::make_signed_t<T>, std::make_signed_t<T>> exgcd(
    const T &a,
    const T &b) {
  static_assert(std::numeric_limits<T>::is_integer,
                "exgcd arguments must be integers");

  using Signed_T = std::make_signed_t<T>;
  using Unsigned_T = std::make_unsigned_t<T>;

  // xa * abs(a) + ya * abs(b) == ta
  // xb * abs(b) + yb * abs(b) == tb
  Unsigned_T ta = unsigned_abs(a), tb = unsigned_abs(b);
  Signed_T xa = 1, ya = 0;
  Signed_T xb = 0, yb = 1;

  while (tb != 0) {
    Signed_T q = numeric_cast<Signed_T>(ta / tb);

    // xc * abs(a) + yc * abs(b) == tc
    Unsigned_T tc = ta - static_cast<Unsigned_T>(q) * tb;
    Signed_T xc = xa - q * xb;
    Signed_T yc = ya - q * yb;

    xa = xb, xb = xc;
    ya = yb, yb = yc;
    ta = tb, tb = tc;
  }

  return std::make_pair(sign(a) * xa, sign(b) * ya);
}

// Computes the value of |base| raised to an integer power |exponent|.
// This version of overload uses binary exponentiation technique to compute in
// O(log |exponent|) time.
template <typename T,
          typename U,
          std::enable_if_t<std::numeric_limits<U>::is_integer, bool> = true>
T pow(T base, const U &exponent) {
  using Pair_TT = std::pair<T, T>;
  using Unsigned_U = std::make_unsigned_t<U>;

  auto update = [](bool bit, Pair_TT &state) {
    // At the n-th bit of |exponent|, |power| is |base|^(2^n), and
    // |result| is the answer for the first n bits of |exponent|.
    auto &[result, power] = state;
    if (bit)
      result *= power;
    power *= power;
  };

  Unsigned_U abs_exp = unsigned_abs(exponent);
  Pair_TT state = binary_accumulate<Unsigned_U, Pair_TT>(
      abs_exp, std::make_pair(T(1), std::move(base)), update);
  T result = std::move(state.first);

  // Return the inverse of the result if the exponent is negative.
  if (exponent < 0)
    return 1 / result;
  return result;
}

// Computes the value of |base| raised to a non-integer power |exponent|.
// This version of overload is the same as std::pow.
template <typename T,
          typename U,
          std::enable_if_t<!std::numeric_limits<U>::is_integer, bool> = true>
auto pow(T base, U exponent) -> decltype(std::pow(base, exponent)) {
  return std::pow(base, exponent);
}

// Returns the |n|-th root of |x| rounded to integer toward zero.
// The argument |n| shoud be positive.
// Throws domain_error exception if the root does not exist.
// The time complexity is O(log |x|).
//
// This overload is only enabled for x < 2^64, because it uses uint64_t
// to avoid overflows during the computation.
template <typename T,
          typename U,
          std::enable_if_t<std::numeric_limits<T>::digits <= 64, bool> = true>
T iroot(const T &x, const U &n) {
  static_assert(
      std::numeric_limits<T>::is_integer && std::numeric_limits<U>::is_integer,
      "iroot arguments must be integers.");
  if (n < 0)
    throw std::invalid_argument("iroot argument |n| should be positive.");
  if (n == 0)
    throw std::domain_error("The 0-th root does not exist.");
  if (x < 0 && n % 2 == 0)
    throw std::domain_error("Negative numbers do not have even roots.");

  if (n == 1)
    return x;

  // For each n, the table stores the maximum y that y^n does not overflow.
  static const int max_size = 64;
  static const uint64_t y_limits[max_size] = {
      0,  0,  4294967295, 2642245, 65535, 7131, 1625, 565, 255, 138, 84, 56, 40,
      30, 23, 19,         15,      13,    11,   10,   9,   8,   7,   6,  6,  5,
      5,  5,  4,          4,       4,     4,    3,    3,   3,   3,   3,  3,  3,
      3,  3,  2,          2,       2,     2,    2,    2,   2,   2,   2,  2,  2,
      2,  2,  2,          2,       2,     2,    2,    2,   2,   2,   2,  2};
  uint64_t y_limit = (n >= max_size ? 1 : y_limits[n]);

  // Binary search for the maximum y that y^n <= abs(x).
  uint64_t abs_x = unsigned_abs(x);
  uint64_t y_min = 0;
  uint64_t y_max = std::min(abs_x, y_limit) + 1;
  while (y_max - y_min > 1) {
    uint64_t y_mid = std::midpoint(y_min, y_max);
    if (pow(y_mid, n) > abs_x)
      y_max = y_mid;
    else
      y_min = y_mid;
  }
  return sign(x) * static_cast<T>(y_min);
}

}  // namespace number_theory

using number_theory::exgcd;
using number_theory::gcd;
using number_theory::iroot;
using number_theory::lcm;
using number_theory::pow;

}  // namespace tql

#endif  // NUMBER_THEORY_NUMERIC_H_
