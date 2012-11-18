#include <cfloat>
#include <iostream>
#include <iomanip>
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
  vector<SignCounter> signCounter(n_features_);
  int iterations = 100000;

  double curCost = DBL_MAX, lastCost = 0.0, costChg = DBL_MAX, signCountPt = 1E-5;
  bool isSignCount = false;
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
    // Investigate cost change magnitude and sign count
    if (isSignCount) {
      signCountDowork(guessW, signCounter);
    }
    lastCost = curCost;
    curCost = costGivenGuessW(xxMatr, scores, numOfCands, guessW);
    costChg = curCost - lastCost;  // Should always be negative
    std::cout << "curCost= " << curCost << "  costChg= " << costChg
      << "  signDiffCount= " << signDiffToExactW(guessW) << std::endl;

    // Start sign counting from next loop
    if (isSignCount == false && (iterations < 40000)) {  // Only count last 10% loops
      isSignCount = true;
      signCountInit(guessW, signCounter);
    }
  }

  printSignCounter(signCounter);
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

// Cheating, compare Exact W with guessW, return total number of sign differences
int GradientMatcher::signDiffToExactW(const double* guessW) const {
  int diffCount = 0;
  const double* exactW = local_game_->getExactWArr_();

  for (int i = 0; i < n_features_; ++i) {
    if ((guessW[i] >= 0.0) ^ (exactW[i] >= 0.0))
      ++diffCount;
  }

  return diffCount;
}

void GradientMatcher::signCountInit(const double* guessW,
    vector<SignCounter>& signCounter) const {
  assert(signCounter.size() == n_features_);

  for (int i = 0; i < n_features_; ++i) {
    signCounter[i].lastSign_ = ((guessW[i] >= 0.0)? true : false);
  }
}

void GradientMatcher::signCountDowork(const double* guessW,
    vector<SignCounter>& signCounter) const {
  assert(signCounter.size() == n_features_);

  for (int i = 0; i < n_features_; ++i) {
    if (guessW[i] >= 0.0) {
      ++(signCounter[i].posCount_);
    } else {
      ++(signCounter[i].negCount_);
    }

    if ((guessW[i] >= 0.0) ^ signCounter[i].lastSign_) {  // sign changed
      ++(signCounter[i].signFliped_);
    }

    signCounter[i].lastSign_ = ((guessW[i] >= 0.0)? true : false);
  }
}

void GradientMatcher::printSignCounter(const vector<SignCounter>& signCounter) const {
  std::cout << "     ";
  for (int i = 0; i < signCounter.size(); ++i)
    std::cout << std::setw(5) << signCounter[i].signFliped_ << " ";
  std::cout << "\nPos: ";
  for (int i = 0; i < signCounter.size(); ++i)
    std::cout << std::setw(5) << signCounter[i].posCount_ << " ";
  std::cout << "\nNeg: ";
  for (int i = 0; i < signCounter.size(); ++i)
    std::cout << std::setw(5) << signCounter[i].negCount_ << " ";
  std::cout << std::endl;
}

