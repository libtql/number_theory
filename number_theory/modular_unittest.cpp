#include <gtest/gtest.h>

#include "number_theory/modular.h"

namespace tql {
namespace number_theory {

TEST(ModularTest, Basic) {
  using Mod10 = Modular<int, 10>;

  // test constructors
  Mod10 a = 123;
  Mod10 b = a;
  Mod10 c;
  EXPECT_EQ(a.get(), 3);
  EXPECT_EQ(b.get(), 3);
  EXPECT_EQ(c.get(), 0);

  // test assignments
  c = -1;
  EXPECT_EQ(c.get(), 9);
  a = std::move(c);
  EXPECT_EQ(a.get(), 9);
  a.set(-4);
  EXPECT_EQ(a.get(), 6);

  // test conversions
  a = 3;
  EXPECT_EQ(static_cast<int>(a), 3);
  EXPECT_EQ(static_cast<Mod10>(15).get(), 5);

  // test arithmetics
  a = 6, b = 4;
  c = a.add(b);
  EXPECT_EQ(c.get(), 0);
  c = b.negate();
  EXPECT_EQ(c.get(), 6);
  c = b.substract(a);
  EXPECT_EQ(c.get(), 8);
  c = a.multiply(b);
  EXPECT_EQ(c.get(), 4);

  // test comparisons
  c = 3;
  b = c;
  a = 0;
  EXPECT_TRUE(b.equal(c));
  EXPECT_FALSE(a.equal(b));
}

}  // namespace number_theory
}  // namespace tql
