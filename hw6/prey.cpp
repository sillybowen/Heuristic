#include "prey.h"

Prey::~Prey() { }

void Prey::output() const {
}

bool Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Prey::tryMove() {
  return true;
}
