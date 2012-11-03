#ifndef JINIL_HUNTER_HEADER_
#define JINIL_HUNTER_HEADER_

#include "moveable.h"

class Jinil_Hunter : public Moveable {
public:
  Jinil_Hunter(int hx, int hy, int nn, int mm) : Moveable(hx, hy, nn, mm) { }
  ~Jinil_Hunter();

  void output() const;
  bool isHunter() const;
  // Jinil_Hunter cannot control his move directly. He can only change his direction by
  // bouncing, so this method is mainly for creating/removing walls
  HuntPreyOutput tryMove();

private:
};

#endif  // HUNTER_HEADER_
