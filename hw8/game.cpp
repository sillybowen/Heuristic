#include <cstring>
#include "game.h"

#define MVALUE 20

Game::Game(const char* plyName, int nFeatures)
                    : n_features_(nFeatures),
                      m_srv_cand_(MVALUE),
                      ply_name_(string(plyName)),
                      w_arr_(new double[nFeatures]),
                      xx_matr_(new double*[MVALUE]) {
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
}

void Game::startGame() {
}
