#ifndef NUMBER_THEORY_MODULAR_H_
#define NUMBER_THEORY_MODULAR_H_

#include <stddef.h>

#include <bit>
#include <limits>
#include <type_traits>

#include "number_theory/utility.h"

// Modular arithmetic

namespace tql {
namespace number_theory {

namespace {

// Returns the equivalent element of |x| in the ring of integers modulo
// |modulus|. The result |y| should statisfy 0 <= y < modulus, and
// y = k*modulus + x for some integer k.
template <class T,
          std::enable_if_t<std::numeric_limits<T>::is_integer, bool> = true>
constexpr T normalize(T x, T modulus) {
  T y = std::move(x);
  if (y < 0 || y >= modulus) {
    y %= modulus;
    if (y < 0)
      y += modulus;
  }
  return y;
}

template <class T>
struct ModulusWrapper {
  using type = T;
  T value;
  constexpr ModulusWrapper(T x) : value(std::move(x)) {}
};

}  // namespace

template <ModulusWrapper mod>
class Modular {
 public:
  using type = std::decay_t<typename decltype(mod)::type>;
  static constexpr type modulus = mod.value;

  static_assert(std::numeric_limits<type>::is_integer && modulus > 0,
                "Modular requires modulus to be a positive integer.");

  // This is an implicit constructor. We implicitly upgrade from Modular::type
  // to Modular to make it easier to use.
  Modular(type value = 0) { set(std::move(value)); }

  Modular(const Modular &other) = default;
  Modular(Modular &&other) = default;
  Modular &operator=(const Modular &other) = default;
  Modular &operator=(Modular &&other) = default;

  // This is an implicit conversion, because we want a seamless conversion from
  // Modular to Modular::type.
  operator type() const { return get(); }

  const type &get() const { return value_; }

  void set(type value) { value_ = normalize(std::move(value), modulus); }

  Modular add(const Modular &rhs) const {
    check_addition_overflow();
    type new_value = value_ + rhs.value_;
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
  type value_;

  static constexpr size_t type_width = std::numeric_limits<type>::digits;
  static constexpr size_t modulus_width =
      std::bit_width(static_cast<std::make_unsigned_t<type>>(modulus));

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
