#ifndef RANDOM_PREY_HEADER_
#define RANDOM_PREY_HEADER_

#include "moveable.h"
#include <time.h>

class Random_Prey : public Moveable {
public:
  Random_Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) { }
  ~Random_Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

private:
};

#endif  // RANDOM_PREY_HEADER_
