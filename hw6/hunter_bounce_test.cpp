#include <iostream>
#include <vector>
#include <string>
#include "moveable.h"
#include "gtest/gtest.h"

namespace {

  using std::cout;
  using std::endl;
  using std::string;
  using std::vector;
  class HunterBounceTest : public ::testing::Test {
    protected:
      // You can do set-up work for each test here.
      HunterBounceTest() : border_len_(500) {
        // Insert game borders as special walls (No thickness)
        hor_walls_.push_back(new Moveable::Wall(0, 0, 500, 0, 0));
        hor_walls_.push_back(new Moveable::Wall(0, 500, 500, 500, 2));
        ver_walls_.push_back(new Moveable::Wall(0, 0, 0, 500, 1));
        ver_walls_.push_back(new Moveable::Wall(500, 0, 500, 500, 3));
      }

      virtual ~HunterBounceTest() {
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
      vector<Moveable::Wall*> hor_walls_;
      vector<Moveable::Wall*> ver_walls_;
  };

  TEST_F(HunterBounceTest, NoWallJust4Borders) {
    const int MaxNumOfSteps = 2000;
    vector<Moveable::Pos> hFutureSteps;
    vector<Moveable::Pos> hPosHis;
    Moveable::Pos hStartPos;
    hStartPos.set(0, 0);

    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis);

    int diagIndex = 0, direction = 1;
    for (int i = 0; i < MaxNumOfSteps; ++i) {
      EXPECT_EQ(diagIndex, hFutureSteps[i].x);
      EXPECT_EQ(diagIndex, hFutureSteps[i].y);
      diagIndex += direction;
      if (diagIndex == border_len_ || diagIndex == 0)
        direction = -direction;
    }
  }

  TEST_F(HunterBounceTest, SimpleHorVertWall) {
    const int MaxNumOfSteps = 1;
    vector<Moveable::Pos> hFutureSteps;
    vector<Moveable::Pos> hPosHis;
    Moveable::Pos hStartPos;
    ver_walls_.push_back(new Moveable::Wall(100, 50, 100, 150, 4));

    // Tests against ver_walls_
    hStartPos.set(99, 99);
    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis);
    EXPECT_EQ(99, hFutureSteps[1].x);
    EXPECT_EQ(100, hFutureSteps[1].y);

    hStartPos.set(99, 49);  // will bounce to left down direction
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(2, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis);
    EXPECT_EQ(99, hFutureSteps[1].x);
    EXPECT_EQ(50, hFutureSteps[1].y);
    EXPECT_EQ(98, hFutureSteps[2].x);
    EXPECT_EQ(51, hFutureSteps[2].y);

    hStartPos.set(99, 150);
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis);
    EXPECT_EQ(100, hFutureSteps[1].x);
    EXPECT_EQ(151, hFutureSteps[1].y);

    hStartPos.set(99, 48);
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(2, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis);
    EXPECT_EQ(100, hFutureSteps[1].x);
    EXPECT_EQ(49, hFutureSteps[1].y);
    EXPECT_EQ(101, hFutureSteps[2].x);
    EXPECT_EQ(50, hFutureSteps[2].y);

  }

  TEST_F(HunterBounceTest, TestAgainstVerticalWall) {
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
