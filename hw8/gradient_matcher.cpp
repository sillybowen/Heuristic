#include <cfloat>
#include <iostream>
#include "gradient_matcher.h"
#include "game.h"

const static double etaArr[] = { 0.01, 0.005, 0.001, 0.0005, 0.0001 };

GradientMatcher::GradientMatcher(const string& plyName, int nFeatures, Game* pgame)
                                : Someone(plyName, nFeatures), local_game_(pgame) { }

GradientMatcher::~GradientMatcher() { }

void GradientMatcher::feedRandCandsResults(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands, double eta, int leaveInd,
    double* retGuessW) const {
  double* gtArr = new double[n_features_];  // Gradient array
  double* guessW = new double[n_features_];
  for (int i = 0; i < n_features_; ++i) guessW[i] = 0.0; 
  int iterations = 10000;

  while (iterations-- > 0) {
    for (int i = 0; i < n_features_; ++i) gtArr[i] = 0.0;
    // Foreach candidate c, calculate gradient
    for (int c = 0; c < numOfCands; ++c) {
      if (c == leaveInd)
        continue;

      double dotprod = 0.0;
      for (int ind = 0; ind < n_features_; ++ind)
        dotprod += guessW[ind] * xxMatr[c][ind];

      double diff = dotprod - scores[c];
      for (int j = 0; j < n_features_; ++j)
        gtArr[j] += diff * xxMatr[c][j];
    }

    // Update guessW
    for (int k = 0; k < n_features_; ++k)
      guessW[k] -= eta * gtArr[k];

    // Print guessed W:
    local_game_->printLenNArr(guessW);
    // double curCost = costGivenGuessW(xxMatr, scores, numOfCands, guessW);
    // std::cout << "curCost= " << curCost << std::endl;
  }

  // Output final guessW to caller
  if (retGuessW) {
    for (int i = 0; i < n_features_; ++i)
      retGuessW[i] = guessW[i];
  }
  delete [] guessW;
  delete [] gtArr;

  // Only for costGivenGuessW testing
  /*
  double noCost =
    costGivenGuessW(xxMatr, scores, numOfCands, local_game_->getExactWArr_());
  std::cout << "noCost(expect 0.0) = " << noCost << std::endl;
  */
}

// Leave-One-Out cross validation, return the best deta rate index
int GradientMatcher::LOOCrossValid(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands) const {
  double bestCost = DBL_MAX;
  double tmpGuessW[n_features_];
  const int etaArrLen = sizeof(etaArr) / sizeof(double);
  int bestEtaInd = 0;

  for (int i = 0; i < etaArrLen; ++i) {
    double curCost = 0.0;
    for (int c = 0; c < numOfCands; ++c) {
      feedRandCandsResults(xxMatr, scores, numOfCands, etaArr[i], c, tmpGuessW);
      curCost += costGivenGuessW(xxMatr, scores, numOfCands, tmpGuessW);
    }
    std::cout << "eta = " << etaArr[i] << "  cost= " << curCost << std::endl;
    if (curCost < bestCost) {
      bestCost = curCost;
      bestEtaInd = i;
    }
  }

  return bestEtaInd;
}

void GradientMatcher::sendOutVector(double* aVector) {
}

// Used to evaluate @guessW
// cost(@guessW) = sum (x_c' * guessW - y_c)^2
double GradientMatcher::costGivenGuessW(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands, const double* guessW) const {
  double totalCost = 0.0;
  // Foreach candidate c, calculate difference
  for (int c = 0; c < numOfCands; ++c) {
    double oneCandCost = 0.0;
    for (int i = 0; i < n_features_; ++i) {
      oneCandCost += xxMatr[c][i] * guessW[i];
    }
    oneCandCost -= scores[c];
    totalCost += oneCandCost * oneCandCost;
  }

  return totalCost;
}
