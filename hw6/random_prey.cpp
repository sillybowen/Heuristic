#include "random_prey.h"

Random_Prey::~Random_Prey() { }

void Random_Prey::output() const {
}

bool Random_Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Random_Prey::tryMove() {
  srand (time(NULL));
  int x = (rand() % 3) -1;
  srand (1);
  int y = (rand() % 3) -1;
  
  return Moveable::HuntPreyOutput(x, y);
}
