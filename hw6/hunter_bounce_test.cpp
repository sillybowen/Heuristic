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
    int h_dir_x, h_dir_y;

    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis, h_dir_x, h_dir_y);

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
    int h_dir_x, h_dir_y;
    ver_walls_.push_back(new Moveable::Wall(100, 50, 100, 150, 4));

    // Tests against ver_walls_
    hStartPos.set(99, 99);
    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis, h_dir_x, h_dir_y);
    EXPECT_EQ(99, hFutureSteps[1].x);
    EXPECT_EQ(100, hFutureSteps[1].y);

    hStartPos.set(99, 49);  // will bounce to left down direction
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(2, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis, h_dir_x, h_dir_y);
    EXPECT_EQ(99, hFutureSteps[1].x);
    EXPECT_EQ(50, hFutureSteps[1].y);
    EXPECT_EQ(98, hFutureSteps[2].x);
    EXPECT_EQ(51, hFutureSteps[2].y);
    EXPECT_EQ(h_dir_x, -1);
    EXPECT_EQ(h_dir_y, 1);

    hStartPos.set(99, 150);
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(MaxNumOfSteps, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis, h_dir_x, h_dir_y);
    EXPECT_EQ(100, hFutureSteps[1].x);
    EXPECT_EQ(151, hFutureSteps[1].y);

    hStartPos.set(99, 48);
    hFutureSteps.clear();
    Moveable::hunterNStepPrediction(2, hFutureSteps, hor_walls_,
        ver_walls_, hStartPos, hPosHis, h_dir_x, h_dir_y);
    EXPECT_EQ(100, hFutureSteps[1].x);
    EXPECT_EQ(49, hFutureSteps[1].y);
    EXPECT_EQ(101, hFutureSteps[2].x);
    EXPECT_EQ(50, hFutureSteps[2].y);
    EXPECT_EQ(h_dir_x, 1);
    EXPECT_EQ(h_dir_y, 1);

  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
