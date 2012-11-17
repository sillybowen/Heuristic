#ifndef gradient_matcher_h
#define gradient_matcher_h
#include "someone.h"

class Game;
class GradientMatcher : public Someone {
public:
  GradientMatcher(const string& plyName, int nFeatures, Game* pgame);
  ~GradientMatcher();

  bool isMatchmaker() const { return true; }
  // Override base class method
  // @leaveInd is for "Leave one out cross validation"
  // @retGuessW should be an array of length nFeatures, if NOT NULL
  void feedRandCandsResults(const double* const* xxMatr,
      const vector<double>& scores, int numOfCands, double eta = 0.0001,
      int leaveInd = -1, double* retGuessW = NULL) const;
  // Override base class method
  void sendOutVector(double* aVector);

  int LOOCrossValid(const double* const* xxMatr, const vector<double>& scores,
      int numOfCands) const;
  double costGivenGuessW(const double* const* xxMatr, const vector<double>& scores,
      int numOfCands, const double* guessW) const;

private:
  Game*  local_game_;  // Cheating, test gradient descent results
};

#endif
