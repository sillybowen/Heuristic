#include <iostream>
#include "hunter.h"
#include "evasion.h"

using std::cout;
using std::endl;

Hunter::~Hunter() { }

void Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Hunter::isHunter() const {
  evade_game_->dumpAdjWallsMatrixGraph();
  return true;
}

Moveable::HuntPreyOutput Hunter::tryMove() {
}
