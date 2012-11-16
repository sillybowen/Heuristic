#include <cmath>
#include <iostream>
#include <string>
#include "person.h"
#include "gtest/gtest.h"

namespace {

  using std::cout;
  using std::endl;
  using std::string;
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

      ASSERT_DOUBLE_EQ(posSum, 1.0);
      ASSERT_DOUBLE_EQ(negSum, -1.0);
    }
  }

  TEST_F(RandomPersonTest, ValidNoises) {
    int numOfTries = 200;
    const int nFeatures = 73;
    double noise[nFeatures];
    Person p("testPlyer", nFeatures);
    const double* exactW = p.getExactW_();
    // First sendOutVector should equal to exactW
    p.sendOutVector(noise);
    for (int i = 0; i < nFeatures; ++i)
      ASSERT_DOUBLE_EQ(noise[i], exactW[i]);

    while (numOfTries-- > 0) {
      int modifiedCount = 0;
      p.sendOutVector(noise);
      for (int i = 0; i < nFeatures; ++i) {
        if (fabs(noise[i]) < 0.01)
          ASSERT_NEAR(0.0, noise[i], 0.01);
        else {
          EXPECT_NEAR(0.0, noise[i], fabs(exactW[i] * 0.2));
          ++modifiedCount;
        }
      }
      EXPECT_LE(modifiedCount, int(nFeatures * 0.05)) << " modifiedCount= "
        << modifiedCount << endl;
    }
  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
