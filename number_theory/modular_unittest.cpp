#include <gtest/gtest.h>

#include "number_theory/modular.h"

namespace tql {

TEST(ModularTest, Basic) {
  using Mod10 = Modular<int, 10>;
  // constructor
  Mod10 a = 123;
  EXPECT_EQ(a.get(), 3);
  // constructor
  Mod10 b = a;
  EXPECT_EQ(b.get(), 3);
  // assign
  Mod10 c;
  c = a;
  EXPECT_EQ(c.get(), 3);
  // set
  a.set(-4);
  EXPECT_EQ(a.get(), 6);
  // add
  c = a.add(b);
  EXPECT_EQ(c.get(), 9);
  // negate
  c = b.negate();
  EXPECT_EQ(c.get(), 7);
  // substract
  c = b.substract(a);
  EXPECT_EQ(c.get(), 7);
  // multiply
  c = a.multiply(b);
  EXPECT_EQ(c.get(), 8);
  // equal
  c = 3;
  EXPECT_TRUE(b.equal(c));
  EXPECT_FALSE(a.equal(b));
}

}  // namespace tql
