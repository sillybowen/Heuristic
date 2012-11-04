#include <string>
#include "moveable.h"
#include "gtest/gtest.h"

namespace {

  using std::string;
// The fixture for testing class Foo.
  class MoveableTest : public ::testing::Test {
    protected:
      // You can remove any or all of the following functions if its body
      // is empty.
      MoveableTest() {
        // You can do set-up work for each test here.
      }

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
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(MoveableTest, Two_Wall_Cross) {
    int x1, y1, x2, y2;
    const string input_filepath = "this/package/testdata/myinputfile.dat";
    const string output_filepath = "this/package/testdata/myoutputfile.dat";
    Moveable::Wall staticWall(100, 100, 200, 100, 0);
    Moveable::Wall testWall(199, 100, 300, 100, 1);

    EXPECT_EQ(0, staticWall.isTwoWallAdjacent(&testWall, x1, y1, x2, y2));
  }

  // Tests that Foo does Xyz.
  TEST_F(MoveableTest, DoesXyz) {
    // Exercises the Xyz feature of Foo.
  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
