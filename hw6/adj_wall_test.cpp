#include <iostream>
#include <string>
#include "moveable.h"
#include "gtest/gtest.h"

namespace {

  using std::cout;
  using std::endl;
  using std::string;
  // The fixture for testing class Foo.
  class MoveableTest : public ::testing::Test {
    protected:
      // You can do set-up work for each test here.
      MoveableTest() : border_len_(500) { }

      virtual ~MoveableTest() {
        // You can do clean-up work that doesn't throw exceptions here.
      }

      // If the constructor and destructor are not enough for setting up
      // and cleaning up each test, you can define the following methods:
      virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
      }

      virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
      }

      // Objects declared here can be used by all tests in the test case for Foo.
      const int border_len_;
  };

  TEST_F(MoveableTest, TestAgainstHorizontalWall) {
    const int wallLen = 5;
    int x1, y1, x2, y2;

    const Moveable::Wall staticWall(100, 100, 200, 100, 0);

    // Two horizontal walls
    for (int i = 0; i + wallLen <= border_len_; ++i) {
      y1 = y2 = 99;
      x1 = i;
      // Two parallel wall with y-1
      Moveable::Wall testUpWall(x1, y1, x1 + wallLen, y2, 1);
      if (i < 99 - wallLen || i > 201)
        EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testUpWall, x1, y1, x2, y2));
      else
        EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testUpWall, x1, y1, x2, y2));

      // Two parallel wall with y+1
      y1 = y2 = 101;
      Moveable::Wall testDownWall(x1, y1, x1 + wallLen, y2, 1);
      if (i < 99 - wallLen || i > 201)
        EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testDownWall, x1, y1, x2, y2));
      else
        EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testDownWall, x1, y1, x2, y2));

      // Two parallel wall with same y
      y1 = y2 = 100;
      Moveable::Wall testCollisionWall(x1, y1, x1 + wallLen, y2, 1);
      if (i < 99 - wallLen || i > 201)
        EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testCollisionWall, x1, y1, x2, y2));
      else if (i == 99 - wallLen || i == 201)
        EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testCollisionWall, x1, y1, x2, y2));
      else
        EXPECT_EQ(-1, staticWall.isTwoWallAdjacent(&testCollisionWall, x1, y1, x2, y2));

      // All other horizontal walls with other y values
      for (int j = 1; j < border_len_; ++j) {
        if (j >= 99 && j <= 101) continue;
        y1 = y2 = j;
        Moveable::Wall testWall(x1, y1, x1 + wallLen, y2, 1);
        EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
      }
    }

    // Test wall vertical
    for (x1 = 1; x1 < border_len_; ++x1) {
      x2 = x1;
      if (x1 < 99 || x1 > 201) {
        for (y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
        }
      } else if (x1 == 99 || x1 == 201) {
        for (int y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          if (y1 < 99 - wallLen || y1 > 101)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2))
              << "(" << x1 << ", " << y1 << ") -> (" << x2 << ", " << y1 + wallLen << ")\n";
          else
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
        }
      } else {  // 99 < x1 < 201
        for (int y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          if (y1 < 99 - wallLen || y1 > 101)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
          else if (y1 == 99 - wallLen || y1 == 101)
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
          else
            EXPECT_EQ(-1, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
        }
      }
    }
  }

  TEST_F(MoveableTest, TestAgainstVerticalWall) {
    const int wallLen = 5;
    int x1, y1, x2, y2;
    int xxa, yya, xxb, yyb;

    const Moveable::Wall staticWall(100, 100, 100, 200, 0);

    // Two vertical walls
    for (x1 = 1; x1 < border_len_; ++x1) {
      x2 = x1;
      if (x1 < 99 || x1 > 101) {  // Two vertical, no intersect
        for (y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      } else if (x1 == 99 || x1 == 101) {  // Two vertical, x+/-1
        for (y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          if (y1 + wallLen < 99 || y1 > 201)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      } else {  // Two vertical, x1 == 100
        for (y1 = 0; y1 + wallLen <= border_len_; ++y1) {
          Moveable::Wall testWall(x1, y1, x2, y1 + wallLen, 1);
          if (y1 + wallLen < 99 || y1 > 201)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else if (y1 + wallLen == 99 || y1 == 201)
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else
            EXPECT_EQ(-1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      }
    }

    // Test wall horizontal
    for (y1 = 1; y1 < border_len_; ++y1) {
      y2 = y1;
      if (y1 < 99 || y1 > 201) {  // testWall up/down staticWall, no intersect
        for (x1 = 0; x1 + wallLen <= border_len_; ++x1) {
          Moveable::Wall testWall(x1, y1, x1 + wallLen, y2, 1);
          EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      } else if (y1 == 99 || y1 == 201) {  // testWall->y1 +/- y1 or y2
        for (x1 = 0; x1 + wallLen <= border_len_; ++x1) {
          Moveable::Wall testWall(x1, y1, x1 + wallLen, y2, 1);
          if (x1 + wallLen < 99 || x1 > 101)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      } else {  // testWall->y1 inbetween y1 and y2
        for (x1 = 0; x1 + wallLen <= border_len_; ++x1) {
          Moveable::Wall testWall(x1, y1, x1 + wallLen, y2, 1);
          if (x1 + wallLen < 99 || x1 > 101)
            EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else if (x1 + wallLen == 99 || x1 == 101)
            EXPECT_EQ(1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
          else
            EXPECT_EQ(-1, staticWall.isTwoWallAdjacent(&testWall, xxa, yya, xxb, yyb));
        }
      }
    }

  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
