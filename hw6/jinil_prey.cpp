#include "jinil_prey.h"

Jinil_Prey::~Jinil_Prey() { }

void Jinil_Prey::output() const {
}

bool Jinil_Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Jinil_Prey::tryMove() {
  return Moveable::HuntPreyOutput(1, 1);
}
