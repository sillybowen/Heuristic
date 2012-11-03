#ifndef PREY_HEADER_
#define PREY_HEADER_

#include "moveable.h"

class Prey : public Moveable {
public:
  Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) { }
  ~Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

private:
};

#endif  // PREY_HEADER_
