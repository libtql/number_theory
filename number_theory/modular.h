#ifndef NUMBER_THEORY_MODULAR_H_
#define NUMBER_THEORY_MODULAR_H_

#include <stddef.h>

#include <bit>
#include <limits>

#include "number_theory/utility.h"

// Modular arithmetic

namespace tql {
namespace number_theory {

template <class T, T mod>
class Modular {
  static_assert(std::numeric_limits<T>::is_integer && mod > 0,
                "Modular requires modulus to be a positive integer.");

 public:
  using type = T;
  static constexpr T modulus = mod;

  // This is an implicit constructor. We implicitly upgrade from T to Modular
  // to make it easier to use.
  Modular(T value = 0) { set(std::move(value)); }

  Modular(const Modular &other) = default;
  Modular(Modular &&other) = default;
  Modular &operator=(const Modular &other) = default;
  Modular &operator=(Modular &&other) = default;

  // This is an implicit conversion, because we want a seamless conversion to T.
  operator T() const { return get(); }

  const T &get() const { return value_; }

  void set(T value) {
    value_ = std::move(value);
    if (value_ < 0 || value_ >= modulus) {
      value_ %= modulus;
      if (value_ < 0)
        value_ += modulus;
    }
  }

  Modular add(const Modular &rhs) const {
    check_addition_overflow();
    T new_value = value_ + rhs.value_;
    if (new_value >= modulus)
      new_value -= modulus;
    return Modular(new_value);
  }

  Modular negate() const {
    if (value_ == 0)
      return *this;
    return Modular(modulus - value_);
  }

  Modular substract(const Modular &rhs) const { return add(rhs.negate()); }

  Modular multiply(const Modular &rhs) const {
    check_multiplication_overflow();
    return Modular(value_ * rhs.value_);
  }

  bool equal(const Modular &rhs) const { return value_ == rhs.value_; }

 protected:
  T value_;

  static constexpr size_t type_width = std::numeric_limits<T>::digits;
  static constexpr size_t modulus_width =
      std::bit_width(static_cast<std::make_unsigned_t<T>>(modulus));

  void check_addition_overflow() const {
    static_assert(
        modulus_width + 1 <= type_width,
        "Modular addition may overflow. Please use larger integer types.");
  }

  void check_multiplication_overflow() const {
    static_assert(modulus_width * 2 <= type_width,
                  "Modular multiplication may overflow. "
                  "Please use larger integer types.");
  }
};

}  // namespace number_theory

using number_theory::Modular;

}  // namespace tql

#endif  // NUMBER_THEORY_MODULAR_H_
