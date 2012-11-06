#include <cstring>
#include <iostream>
#include <iomanip>
#include "prey.h"
using std::cout;
using std::endl;
using std::setw;

Prey::Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) {
  for (int i = 0; i <= 500; ++i) {
    memset(dangerMap[i], 0, sizeof(int) * 501);
  }
}

Prey::~Prey() { }

void Prey::output() const {
  for (int i = 0; i <= 500; ++i) {
    for (int j = 0; j <= 500; ++j) {
      cout << setw(5) << dangerMap[i][j];
    }
    cout << endl;
  }
}

bool Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Prey::tryMove() {
  return Moveable::HuntPreyOutput(1, 1);
}
