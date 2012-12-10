#include <cfloat>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "callback.hpp"
#include "gradient_matcher.h"
#include "game.h"
#include "person.h"  // For Person::randWeightsGenerator method
#include "projection.h"  // Project gradient vector, to obey constraints
#define MAXCANDIDATESNUMBER 50
#define FINALCOSTTHRESHOLD 0.0001
#define VARTHRESHOLD 0.001
#define NUMOFINITDESCENDPOINTS 180
#define SENDFULLVECTORINTERVAL 7
using std::pair;
using base::Callback;
using base::makeCallableOnce;
using base::ThreadPoolNormal;

const static double etaArr[] = { 0.01, 0.005, 0.001, 0.0005, 0.0001 };

GradientMatcher::GradientMatcher(const string& plyName, int nFeatures, int numThrs,
    Game* pgame) : Someone(plyName, nFeatures),
                   xx_len_(20),
                   matcher_step_(0),
                   num_thrs_(numThrs),
                   xx_matr_(new double*[MAXCANDIDATESNUMBER]),
                   local_game_(pgame),
                   thr_pool_(new ThreadPoolNormal(num_thrs_)) {
  memset(xx_matr_, 0, sizeof(double*) * MAXCANDIDATESNUMBER);
}

GradientMatcher::~GradientMatcher() {
  for (int i = 0; i < xx_len_; ++i)
    delete [] xx_matr_[i];
  delete xx_matr_;

  thr_pool_->stop();
  delete thr_pool_;
}

void GradientMatcher::importRandCandsAndScores(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands) {
  xx_scores_.resize(numOfCands);
  for (int i = 0; i < numOfCands; ++i) {
    xx_matr_[i] = new double[n_features_];
    for (int j = 0; j < n_features_; ++j) {
      xx_matr_[i][j] = xxMatr[i][j];
    }
    xx_scores_[i] = scores[i];
  }
  xx_len_ = numOfCands;
}

static int finishedCounter = 0;

// Do gradient descend from multiple starting points
void GradientMatcher::descendFromMultiSPs() {
  printXXMatrWithScore(xx_len_);
  mul_desc_.clear();
  for (int i = 0; i < NUMOFINITDESCENDPOINTS; ++i) {
    mul_desc_.push_back(new MulDesc(n_features_));
  }
  sign_count_.clear();
  sign_count_.resize(n_features_);

  WaitAllDone* waitAllDone = new WaitAllDone();

  finishedCounter = 0;
  for (int ind = 0; ind < NUMOFINITDESCENDPOINTS; ++ind) {
    Person::randWeightsGenerator(mul_desc_[ind]->guessWArr, n_features_);

    Callback<void>* task = makeCallableOnce(&GradientMatcher::feedRandCandsResults,
        this, mul_desc_[ind], 0.001, -1, (double*)NULL);
    thr_pool_->addTask(task);
    // feedRandCandsResults(mul_desc_.back()->guessWArr);
  }
  Callback<void>* allDone = makeCallableOnce(&WaitAllDone::jobsDoneSignal,
      waitAllDone);
  thr_pool_->addTask(allDone);
  waitAllDone->wait();

  std::cerr << "-----------------------------" << "NumFinished: " << finishedCounter
    << std::endl;
  // signCountDowork(mul_desc_.back()->guessWArr, signCounter);
  printSignCounter(sign_count_);
  delete waitAllDone;
}

void GradientMatcher::feedRandCandsResults(MulDesc* mulDesc, double eta,
    int leaveInd, double* retGuessW) {
  double* guessW = mulDesc->guessWArr;
  // Print starting guessW array
  std::cerr << "Start descend Weights:\n";
  // local_game_->printLenNArr(guessW);
  double* gtArr = new double[n_features_];  // Gradient array
  // vector<SignCounter> signCounter(n_features_);
  int iterations = 20000;
  __sync_fetch_and_add(&finishedCounter, 0x1);

  double curCost = DBL_MAX, lastCost = 0.0, costChg = DBL_MAX, signCountPt = 1E-5;
  while (iterations-- > 0) {
    for (int i = 0; i < n_features_; ++i) gtArr[i] = 0.0;
    // Foreach candidate c, calculate gradient
    for (int c = 0; c < xx_len_; ++c) {
      if (c == leaveInd)
        continue;

      double dotprod = 0.0;
      for (int ind = 0; ind < n_features_; ++ind)
        dotprod += guessW[ind] * xx_matr_[c][ind];

      double diff = dotprod - xx_scores_[c];
      for (int j = 0; j < n_features_; ++j)
        gtArr[j] += diff * xx_matr_[c][j];
    }
    // Update guessW
    for (int k = 0; k < n_features_; ++k)
      guessW[k] -= eta * gtArr[k];

    // Project guessW to [1,1,1...1] hyperplane
    Projection::projForPosNegConstraint(guessW, n_features_);

    // Print guessed W:
    // local_game_->printLenNArr(guessW);
    lastCost = curCost;
    curCost = costGivenGuessW(xx_len_, guessW);  // ******@xx_len_ is CONSTANT now
    costChg = curCost - lastCost;  // Should always be negative
    // std::cout << "curCost= " << curCost << "  costChg= " << costChg
    //   << "  signDiffCount= " << signDiffToExactW(guessW) << std::endl;
    if (curCost < FINALCOSTTHRESHOLD)
      break;
  }

  mulDesc->finalCost = curCost;
  // std::cerr << "signDiffToExactW: " << signDiffToExactW(guessW) << " ConstrainInfo:";
  std::cerr << "ConstrainInfo: ";
  outputConstraintInfo(mulDesc);  // ----Update sign_count_ here
  // printSignCounter(signCounter);
  // Output final guessW to caller
  if (retGuessW) {
    for (int i = 0; i < n_features_; ++i)
      retGuessW[i] = guessW[i];
  }
  delete [] gtArr;

}

/*
// Leave-One-Out cross validation, return the best deta rate index
int GradientMatcher::LOOCrossValid(const double* const* xxMatr,
    const vector<double>& scores, int numOfCands) {
  double bestCost = DBL_MAX;
  double tmpGuessW[n_features_];
  const int etaArrLen = sizeof(etaArr) / sizeof(double);
  int bestEtaInd = 0;
  double guessW[n_features_];

  for (int i = 0; i < etaArrLen; ++i) {
    double curCost = 0.0;
    for (int c = 0; c < numOfCands; ++c) {
      feedRandCandsResults(guessW, etaArr[i], c, tmpGuessW);
      curCost += costGivenGuessW(numOfCands, tmpGuessW);
    }
    std::cout << "eta = " << etaArr[i] << "  cost= " << curCost << std::endl;
    if (curCost < bestCost) {
      bestCost = curCost;
      bestEtaInd = i;
    }
  }

  return bestEtaInd;
}
*/

// Will increase @xx_len_
void GradientMatcher::sendOutVector(double* aVector) {
  // Clear and resized mul_desc_ and sign_count_
  descendFromMultiSPs();
  ++matcher_step_;

  double* tmpArr = new double[n_features_];
  // Choose "least certain weight wi", then output
  if (matcher_step_ % SENDFULLVECTORINTERVAL && matcher_step_ < 20) {
    int leastKnowWInd = 0;
    int leastAbsCount = INT_MAX, tmp;
    for (int i = 0; i < sign_count_.size(); ++i) {
      if ((tmp = abs(sign_count_[i].posCount_ - sign_count_[i].negCount_))
          < leastAbsCount && (conf_map_.find(i) == conf_map_.end())) {
        leastAbsCount = tmp;
        leastKnowWInd = i;
      }
    }
    // Update @aVector
    for (int i = 0; i < n_features_; ++i) {
      (i == leastKnowWInd)? (aVector[i] = 1.0) : (aVector[i] = 0.0);
      tmpArr[i] = aVector[i];
    }
  } else {  // Output contest array by sign_count_ and conf_map_
    int assignedArr[n_features_];
    memset(assignedArr, 0, sizeof(int) * n_features_);
    ConfirmedMap::const_iterator cit = conf_map_.begin();
    while (cit != conf_map_.end()) {
      aVector[cit->first] = ((cit->second)? 1.0 : 0.0);
      tmpArr[cit->first] = aVector[cit->first];
      assignedArr[cit->first] = 1;
      ++cit;
    }
    // Update rest according to sign_count_
    for (int i = 0; i < n_features_; ++i) {
      if (!(assignedArr[i])) {  // Un-assigned by conf_map_
        aVector[i] = (((sign_count_[i].posCount_ - sign_count_[i].negCount_) >= 0)?
            1.0 : 0.0);
        tmpArr[i] = aVector[i];
        assignedArr[i] = 1;
      }
    }
  }

  xx_matr_[xx_len_++] = tmpArr;
  if (local_game_ != NULL)
    local_game_->printLenNArr(aVector);
}

// TODO: update conf_map_
void GradientMatcher::gotValueForJustSentCand(double newValue) {
  xx_scores_.push_back(newValue);
  assert(int(xx_scores_.size()) == xx_len_);

  // Check if the last inserted was the "prob-array"---only one '1', other all zeros
  int oneCounter = 0, onePos = 0;
  for (int i = 0; i < n_features_; ++i) {
    if (xx_matr_[xx_len_ - 1][i] == 1) {
      ++oneCounter;
      onePos = i;
    }
  }
  if (oneCounter == 1) {
    if (newValue >= 0.0) {  // confirmed w_onePos is >= 0.0
      conf_map_.insert(pair<int, bool>(onePos, true));
    } else {
      conf_map_.insert(pair<int, bool>(onePos, false));
    }
  }
}

// Used to evaluate @guessW
// cost(@guessW) = sum (x_c' * guessW - y_c)^2
double GradientMatcher::costGivenGuessW(int numOfCands, const double* guessW) const {
  double totalCost = 0.0;
  // Foreach candidate c, calculate difference
  for (int c = 0; c < numOfCands; ++c) {
    double oneCandCost = 0.0;
    for (int i = 0; i < n_features_; ++i) {
      oneCandCost += xx_matr_[c][i] * guessW[i];
    }
    oneCandCost -= xx_scores_[c];
    totalCost += oneCandCost * oneCandCost;
  }

  return totalCost;
}

// Output constraint info to stderr
void GradientMatcher::outputConstraintInfo(MulDesc* mulDesc) {
  double* guessW = mulDesc->guessWArr;
  double posSum = 0.0, negSum = 0.0, variance = 0.0;

  for (int i = 0; i < n_features_; ++i) {
    (guessW[i] >= 0.0)? (posSum += guessW[i]) : (negSum += guessW[i]);
  }
  variance = (posSum - 1.0) * (posSum - 1.0) + (negSum + 1.0) * (negSum + 1.0);
  mulDesc->variance = variance;

  // If it can converge from this start point and variance is small
  // Do Concensus(sign count) on this gradient descend
  if ((mulDesc->finalCost <= FINALCOSTTHRESHOLD) && (variance <= VARTHRESHOLD)) {
    signCountDowork(mulDesc->guessWArr, sign_count_);
  }

  std::cerr << "posSum= " << posSum << " negSum= " << negSum << " var= "
    << mulDesc->variance << "  finalCost= " << mulDesc->finalCost << std::endl;
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
  // std::cout << "     ";
  // for (int i = 0; i < signCounter.size(); ++i)
  //   std::cout << std::setw(5) << signCounter[i].signFliped_ << " ";
  // std::cout << "\nPos: ";
  // for (int i = 0; i < signCounter.size(); ++i)
  //   std::cout << std::setw(5) << signCounter[i].posCount_ << " ";
  std::cerr << "\nP-N: ";
  for (int i = 0; i < signCounter.size(); ++i)
    std::cerr << std::setw(5) << signCounter[i].posCount_ - signCounter[i].negCount_
      << " ";
  std::cerr << "\nnumVoters: ";
  for (int i = 0; i < signCounter.size(); ++i)
    std::cerr << std::setw(5) << signCounter[i].posCount_ + signCounter[i].negCount_
      << " ";
  std::cerr << std::endl;
}


void GradientMatcher::printXXMatrWithScore(int numOfCands) const {
  std::cout << "First M random candidates with match score:\n";
  for (int i = 0; i < numOfCands; ++i) {
    for (int j = 0; j < n_features_; ++j) {
      std::cout << std::setw(4) << xx_matr_[i][j] << " ";
    }
    std::cout << "  Score: " << xx_scores_[i] << std::endl;
  }
}
