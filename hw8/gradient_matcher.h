#ifndef gradient_matcher_h
#define gradient_matcher_h
#include "someone.h"

class Game;
class GradientMatcher : public Someone {
public:
  GradientMatcher(const string& plyName, int nFeatures, Game* pgame);
  ~GradientMatcher();

  bool isMatchmaker() const { return true; }
  void feedRandCandsResults(const double* const* xxMatr,
      const vector<double>& scores, int numOfCands);
  void sendOutVector(double* aVector);

private:
  Game*  local_game_;  // Cheating, test gradient descent results
};

#endif
