#include "prey.h"

Prey::~Prey() { }

void Prey::output() const {
}

bool Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Prey::tryMove() {
  return Moveable::HuntPreyOutput(1, 1);
}
