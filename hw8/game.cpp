#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "game.h"
#include "person.h"
#include "matchmaker.h"
using namespace std;

#define MVALUE 20
#define CANDIDATEPRECISION 100

Game::Game(const char* plyName, int nFeatures)
                    : n_features_(nFeatures),
                      m_srv_cand_(MVALUE),
                      ply_name_(string(plyName)),
                      w_arr_(new double[nFeatures]),
                      xx_matr_(new double*[MVALUE]),
                      person_(new Person("Person", nFeatures)),
                      matcher_(new Matchmaker("Matchmaker", nFeatures)) {
  memset(w_arr_, 0, sizeof(double) * nFeatures);
  for (int i = 0; i < MVALUE; ++i) {
    xx_matr_[i] = new double[nFeatures];
    memset(xx_matr_[i], 0, sizeof(double) * nFeatures);
  }
}

Game::~Game() {
  delete [] w_arr_;
  for (int i = 0; i < MVALUE; ++i) {
    delete [] xx_matr_[i];
  }
  delete [] xx_matr_;
  match_score_.clear();
  delete person_;
  delete matcher_;
}

void Game::getMRandomCands() {
  for (int i = 0; i < m_srv_cand_; ++i) {
    for (int j = 0; j < n_features_; ++j) {
      xx_matr_[i][j] = double(rand() % (CANDIDATEPRECISION + 1))
        / double(CANDIDATEPRECISION);
    }
    match_score_.push_back(calMatchScore(xx_matr_[i]));
  }
}

double Game::calMatchScore(const double* candVect) {
  double sum = 0.0;
  for (int i = 0; i < n_features_; ++i)
    sum += candVect[i] * w_arr_[i];
  return sum;
}

double Game::calNoiseMatchScore(const double* candVect, const double* noiseArr) {
  double sum = 0.0;
  for (int i = 0; i < n_features_; ++i)
    sum += candVect[i] * (w_arr_[i] + noiseArr[i]);
  return sum;
}

void Game::startGame() {
  // Get w vector from @person_
  person_->sendOutVector(w_arr_);
  printLenNArr(w_arr_);
  getMRandomCands();
  printXXMatrWithScore();

  int gameRounds = 20;
  double noiseArr[n_features_];
  double newCand[n_features_];
  while (gameRounds-- > 0) {
    person_->sendOutVector(noiseArr);  // Get noise array
    cout << "==============\nSecret, noiseArr: ";
    printLenNArr(noiseArr);
    // Ask for a new candidate from matchmaker
    matcher_->sendOutVector(newCand);
    cout << "newCand: ";
    printLenNArr(newCand);
    double score = calNoiseMatchScore(newCand, noiseArr);
    cout << "Match Score: " << score << endl;
    if (fabs(score - 1.0) < 0.01)
      break;
  }
}

void Game::printLenNArr(const double* lenNArr) const {
  for (int i = 0; i < n_features_; ++i)
    cout << lenNArr[i] << " ";
  cout << endl;
}

void Game::printXXMatrWithScore() const {
  cout << "First M random candidates with match score:\n";
  for (int i = 0; i < m_srv_cand_; ++i) {
    for (int j = 0; j < n_features_; ++j) {
      cout << setw(4) << xx_matr_[i][j] << " ";
    }
    cout << "  Score: " << match_score_[i] << endl;
  }
}
