#include <iostream>
#include "bowen_hunter.h"

using std::cout;
using std::endl;

Bowen_Hunter::~Bowen_Hunter() { }

void Bowen_Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Bowen_Hunter::isHunter() const {
  return true;
}

Moveable::HuntPreyOutput Bowen_Hunter::tryMove() {
}
