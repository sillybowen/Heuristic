#ifndef gradient_matcher_h
#define gradient_matcher_h
#include <vector>
#include "someone.h"
using std::vector;

class Game;
class GradientMatcher : public Someone {
public:
  GradientMatcher(const string& plyName, int nFeatures, Game* pgame);
  ~GradientMatcher();

  struct SignCounter {
    int  signFliped_;
    int  posCount_;
    int  negCount_;
    bool lastSign_;  // if >= 0.0, true; else false
    SignCounter() : signFliped_(0), posCount_(0), negCount_(0) { }
  };
  struct MulDesc {
    double* guessWArr;
    MulDesc(int nFeatures) : guessWArr(new double[nFeatures]) {
      for (int i = 0; i < nFeatures; ++i) guessWArr[i] = 0.0;
    }
    ~MulDesc() { delete [] guessWArr; }
  };

  void importRandCandsAndScores(const double* const* xxMatr,
      const vector<double>& scores, int numOfCands);
  // Override base class method
  void descendFromMultiSPs();
  bool isMatchmaker() const { return true; }
  // @leaveInd is for "Leave one out cross validation"
  // @retGuessW should be an array of length nFeatures, if NOT NULL
  void feedRandCandsResults(double* guessW, double eta = 0.001, int leaveInd = -1,
      double* retGuessW = NULL) const;
  // Override base class method
  void sendOutVector(double* aVector);

  int LOOCrossValid(const double* const* xxMatr, const vector<double>& scores,
      int numOfCands) const;
  double costGivenGuessW(int numOfCands, const double* guessW) const;
  // Cheating, can NOT be used in contest
  int signDiffToExactW(const double* guessW) const;

  // For testing
  void printXXMatrWithScore(int numOfCands) const;

private:
  void signCountInit(const double* guessW, vector<SignCounter>& signCounter) const;
  void signCountDowork(const double* guessW, vector<SignCounter>& signCounter) const;
  void printSignCounter(const vector<SignCounter>& signCounter) const;

  int            xx_len_;  // Number of row currently in @xx_matr_
  double**       xx_matr_;
  Game*  local_game_;  // Cheating, test gradient descent results
  vector<double> xx_scores_;
  vector<MulDesc*> mul_desc_;
};

#endif
