#ifndef BOWEN_HUNTER_HEADER_
#define BOWEN_HUNTER_HEADER_

#include "moveable.h"

class Bowen_Hunter : public Moveable {
public:
  Bowen_Hunter(int hx, int hy, int nn, int mm) : Moveable(hx, hy, nn, mm) { }
  ~Bowen_Hunter();

  void output() const;
  bool isHunter() const;
  // Bowen_Hunter cannot control his move directly. He can only change his direction by
  // bouncing, so this method is mainly for creating/removing walls
  HuntPreyOutput tryMove();

private:
};

#endif  // HUNTER_HEADER_
