#ifndef NUMBER_THEORY_NUMERIC_H_
#define NUMBER_THEORY_NUMERIC_H_

#include <limits>
#include <numeric>
#include <utility>

// Common numeric functions.

namespace number_theory {

using std::gcd;
using std::lcm;

// Extended Euclidean algorithm.
// Given two numbers a and b, returns a pair (x, y) satisfying
// x*a + y*b == gcd(a,b).
template <class T>
std::pair<T, T> exgcd(T a, T b) {
  static_assert(
      std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed,
      "exgcd arguments must be signed integers");

  // xa * a + ya * b == ta
  // xb * b + yb * b == tb
  T ta = a, tb = b;
  T xa = 1, ya = 0;
  T xb = 0, yb = 1;

  while (tb != 0) {
    T q = ta / tb;

    // xc * a + yc * b == tc
    T tc = ta - q * tb;
    T xc = xa - q * xb;
    T yc = ya - q * yb;

    xa = xb, xb = xc;
    ya = yb, yb = yc;
    ta = tb, tb = tc;
  }

  return std::make_pair(xa, ya);
}

}  // namespace number_theory

#endif  // NUMBER_THEORY_NUMERIC_H_