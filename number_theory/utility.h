#ifndef NUMBER_THEORY_UTILITY_H_
#define NUMBER_THEORY_UTILITY_H_

#include <functional>
#include <limits>

// Public utility functions.

namespace tql {
namespace number_theory {

// Accumulates values according to the binary representation of |binary|.
// If |binary| is negative, the two's complement representation will be used.
// For each bit of |binary|, from lower to higher, it applies |operation| to
// accumulate the values. The initial value is |initial_value|.
// As an example, when |operation| is addition, it becomes a popcount.
template <class T, class U>
U binary_accumulate(T binary,
                    const U &initial_value,
                    std::function<void(bool, U &)> operation) {
  static_assert(std::numeric_limits<T>::is_integer,
                "binary_accumulate argument |binary| must be an integer");

  T current = binary;
  U result = initial_value;
  while (current != 0) {
    bool bit = (current % 2) != 0;
    current /= 2;
    operation(bit, result);
  }
  return result;
}

}  // namespace number_theory
}  // namespace tql

#endif  // NUMBER_THEORY_UTILITY_H_
