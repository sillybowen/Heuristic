#include "bowen_prey.h"

Bowen_Prey::~Bowen_Prey() { }

void Bowen_Prey::output() const {
}

bool Bowen_Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Bowen_Prey::tryMove() {
  return Moveable::HuntPreyOutput(1, 1);
}
