#ifndef BOWEN_PREY_HEADER_
#define BOWEN_PREY_HEADER_

#include "moveable.h"

class Bowen_Prey : public Moveable {
public:
  Bowen_Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) { }
  ~Bowen_Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();

private:
};

#endif  // PREY_HEADER_
