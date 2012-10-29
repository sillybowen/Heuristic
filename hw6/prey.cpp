#include "prey.h"

Prey::~Prey() { }

void Prey::output() const {
}

bool Prey::isHunter() const { return false; }

bool Prey::tryMove() {
  return true;
}
