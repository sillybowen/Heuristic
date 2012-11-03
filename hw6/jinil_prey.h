#ifndef JINIL_PREY_HEADER_
#define JINIL_PREY_HEADER_

#include "moveable.h"

class Jinil_Prey : public Moveable {
public:
  Jinil_Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm) { }
  ~Jinil_Prey();

  void output() const;
  bool isHunter() const;
  HuntPreyOutput tryMove();
private:
};

#endif  // PREY_HEADER_
