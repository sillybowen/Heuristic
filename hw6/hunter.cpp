#include <iostream>
#include "hunter.h"

using std::cout;
using std::endl;

Hunter::~Hunter() { }

void Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Hunter::isHunter() const {
  return true;
}

Moveable::HuntPreyOutput Hunter::tryMove() {
}
