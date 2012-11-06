#ifndef PREY_HEADER_
#define PREY_HEADER_

#include "moveable.h"

class Prey : public Moveable {
public:
  Prey(int px, int py, int nn, int mm);
  ~Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

private:
  int dangerMap[501][501];  // high value means high chance to be captured
};

#endif  // PREY_HEADER_
