#include <iostream>
#include <vector>
#include <string>
#include "person.h"
#include "gtest/gtest.h"

namespace {

  using std::cout;
  using std::endl;
  using std::string;
  using std::vector;
  class RandomPersonTest : public ::testing::Test {
    protected:
      // You can do set-up work for each test here.
      RandomPersonTest() {
      }

      virtual ~RandomPersonTest() {
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

  TEST_F(RandomPersonTest, ValidWVect) {
    const int numOfTries = 200;

    for (int i = 0; i < numOfTries; ++i) {
      int nFeatures = rand() % 73 + 20;
      Person p("testPlyer", nFeatures);
      const double* w = p.getExactW_();
      double posSum = 0.0, negSum = 0.0;

      for (int i = 0; i < nFeatures; ++i) {
        if (w[i] >= 0.0) posSum += w[i];
        else negSum += w[i];
      }

      ASSERT_FLOAT_EQ(posSum, 1.0);
      ASSERT_FLOAT_EQ(negSum, -1.0);
    }
  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
