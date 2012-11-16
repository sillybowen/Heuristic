#include <cstdlib>
#include <ctime>
#include "matchmaker.h"

#define PRECISIONPRIMENUMBER 1046527

Matchmaker::Matchmaker(const string& plyName, int nFeatures)
                      : Someone(plyName, nFeatures) { }

Matchmaker::~Matchmaker() { }

bool Matchmaker::isMatchmaker() const { return true; }

void Matchmaker::sendOutVector(double* aVector) {
  srand(time(NULL));
  for (int i = 0; i < n_features_; ++i)
    aVector[i] = double(rand() % PRECISIONPRIMENUMBER)
      / double(PRECISIONPRIMENUMBER - 1);
}
