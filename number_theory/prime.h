#ifndef NUMBER_THEORY_PRIME_H_
#define NUMBER_THEORY_PRIME_H_

#include <limits>

// This file contains functions related to prime numbers.

namespace number_theory {

// Returns true if |number| is prime.
template <class T>
bool is_prime(T number) {
  static_assert(std::numeric_limits<T>::is_integer,
                "is_prime argument must be an integer.");
  // There are no prime numbers smaller than 2.
  if (number < 2)
    return false;
  for (T i = 2; i * i <= number; ++i) {
    if (number % i == 0)
      return false;
  }
  return true;
}

}  // namespace number_theory

#endif  // NUMBER_THEORY_PRIME_H_
