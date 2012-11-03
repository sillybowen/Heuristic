#include <iostream>
#include "jinil_hunter.h"

using std::cout;
using std::endl;

Jinil_Hunter::~Jinil_Hunter() { }

void Jinil_Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Jinil_Hunter::isHunter() const {
  return true;
}

Moveable::HuntPreyOutput Jinil_Hunter::tryMove() {
}
