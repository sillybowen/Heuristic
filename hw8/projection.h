#ifndef projection_h
#define projection_h
#include <cmath>

// Project to [1,1,1...1], "len" of 1s

struct Projection {
  static void projForPosNegConstraint(double* aVector, int len) {
    double avg = 0.0;
    for (int i = 0; i < len; ++i) {
      avg += aVector[i];
    }
    avg /= double(len);
    for (int i = 0; i < len; ++i) {
      aVector[i] -= avg;
    }
  }
};

#endif  // projection_h
