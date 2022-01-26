#include <gtest/gtest.h>

#include "number_theory/modular.h"

namespace tql {
namespace number_theory {

TEST(ModularTest, Basic) {
  using Mod10 = Modular<int, 10>;

  // test constructors
  Mod10 a = 123;
  EXPECT_EQ(a.get(), 3);
  Mod10 b = a;
  EXPECT_EQ(b.get(), 3);
  Mod10 c;

  // test conversions
  EXPECT_EQ(static_cast<int>(a), 3);
  EXPECT_EQ(static_cast<Mod10>(15).get(), 5);

  // test assignments
  c = -1;
  EXPECT_EQ(c.get(), 9);
  c = std::move(a);
  EXPECT_EQ(c.get(), 3);
  a.set(-4);
  EXPECT_EQ(a.get(), 6);

  // test arithmetics
  c = a.add(b);
  EXPECT_EQ(c.get(), 9);
  c = b.negate();
  EXPECT_EQ(c.get(), 7);
  c = b.substract(a);
  EXPECT_EQ(c.get(), 7);
  c = a.multiply(b);
  EXPECT_EQ(c.get(), 8);

  // test comparisons
  c = 3;
  EXPECT_TRUE(b.equal(c));
  EXPECT_FALSE(a.equal(b));
}

}  // namespace number_theory
}  // namespace tql
