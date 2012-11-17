#include "gradient_matcher.h"
#include "game.h"

#define DETAPARAMETER 0.0001

GradientMatcher::GradientMatcher(const string& plyName, int nFeatures, Game* pgame)
                                : Someone(plyName, nFeatures), local_game_(pgame) { }

GradientMatcher::~GradientMatcher() { }

void GradientMatcher::feedRandCandsResults(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands) {
  double* gtArr = new double[n_features_];  // Gradient array
  double* guessW = new double[n_features_];
  for (int i = 0; i < n_features_; ++i) guessW[i] = 0.0; 
  int iterations = 100000;

  while (iterations-- > 0) {
    for (int i = 0; i < n_features_; ++i) gtArr[i] = 0.0;
    // Foreach candidate c, calculate gradient
    for (int c = 0; c < numOfCands; ++c) {
      double dotprod = 0.0;
      for (int ind = 0; ind < n_features_; ++ind)
        dotprod += guessW[ind] * xxMatr[c][ind];

      double diff = dotprod - scores[c];
      for (int j = 0; j < n_features_; ++j)
        gtArr[j] += diff * xxMatr[c][j];
    }

    // Update guessW
    for (int k = 0; k < n_features_; ++k)
      guessW[k] -= DETAPARAMETER * gtArr[k];

    // Print guessed W:
    local_game_->printLenNArr(guessW);
  }

  delete [] guessW;
  delete [] gtArr;
}

void GradientMatcher::sendOutVector(double* aVector) {
}
